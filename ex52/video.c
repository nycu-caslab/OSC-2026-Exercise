/* QEMU ramfb driver implementation */

#include <stdint.h>

extern int strncmp(const char* s1, const char* s2, int n);
extern void* memcpy(void* dst, const void* src, int n);

#define FB_BASE   0xfe000000
#define FB_WIDTH  1920
#define FB_HEIGHT 1080
#define FB_BPP    4
#define XRGB8888  875713112

#define QEMU_PACKED __attribute__((packed))
#define bswap64(x)  __builtin_bswap64(x)
#define bswap32(x)  __builtin_bswap32(x)
#define bswap16(x)  __builtin_bswap16(x)

struct QEMU_PACKED RAMFBCfg {
    uint64_t addr;
    uint32_t fourcc;
    uint32_t flags;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
};

#define FW_CFG_BASE   0x10100000UL
#define FW_CFG_SELECT (uint16_t*)(FW_CFG_BASE + 0x08)
#define FW_CFG_DATA   (uint64_t*)(FW_CFG_BASE + 0x00)
#define FW_CFG_DMA    (uint64_t*)(FW_CFG_BASE + 0x10)

#define FW_CFG_DMA_CTL_ERROR  0x01
#define FW_CFG_DMA_CTL_READ   0x02
#define FW_CFG_DMA_CTL_SKIP   0x04
#define FW_CFG_DMA_CTL_SELECT 0x08
#define FW_CFG_DMA_CTL_WRITE  0x10

#define FW_CFG_FILE_DIR 0x19

struct QEMU_PACKED FWCfgFile {
    uint32_t size;
    uint16_t select;
    uint16_t reserved;
    char name[56];
};

struct QEMU_PACKED FWCfgFiles {
    uint32_t count;
    struct FWCfgFile f[];
};

struct QEMU_PACKED FWCfgDmaAccess {
    uint32_t control;
    uint32_t length;
    uint64_t address;
};

static void fw_cfg_dma_transfer(void* address,
                                uint32_t length,
                                uint32_t control) {
    struct FWCfgDmaAccess access = {
        .control = bswap32(control),
        .length = bswap32(length),
        .address = bswap64((uint64_t)address),
    };
    *FW_CFG_DMA = bswap64((uint64_t)&access);
    while (bswap32(access.control) & ~FW_CFG_DMA_CTL_ERROR)
        ;
}

static void fw_cfg_read_entry(void* buf, int e, int len) {
    uint32_t control = (e << 16) | FW_CFG_DMA_CTL_SELECT | FW_CFG_DMA_CTL_READ;
    fw_cfg_dma_transfer(buf, len, control);
}

static void fw_cfg_write_entry(void* buf, int e, int len) {
    uint32_t control = (e << 16) | FW_CFG_DMA_CTL_SELECT | FW_CFG_DMA_CTL_WRITE;
    fw_cfg_dma_transfer(buf, len, control);
}

static int fw_cfg_find_file(const char* name) {
    uint32_t count = 0;
    fw_cfg_read_entry(&count, FW_CFG_FILE_DIR, sizeof(count));
    count = bswap32(count);
    for (int i = 0; i < count; i++) {
        struct FWCfgFile file;
        fw_cfg_dma_transfer(&file, sizeof(file), FW_CFG_DMA_CTL_READ);
        if (strncmp(name, file.name, sizeof(file.name)) == 0)
            return bswap16(file.select);
    }
    return -1;
}

#define CACHE_BLOCK_SIZE 64
#define cbo_flush(start)                            \
    ({                                              \
        unsigned long __v = (unsigned long)(start); \
        __asm__ __volatile__(                       \
            "cbo.flush"                             \
            " 0(%0)"                                \
            :                                       \
            : "rK"(__v)                             \
            : "memory");                            \
    })

static void flush_dcache(void* addr, unsigned long len) {
    unsigned long start = (unsigned long)addr & ~(CACHE_BLOCK_SIZE - 1);
    __sync_synchronize();
    for (unsigned long line = start; line < (unsigned long)addr + len;
         line += CACHE_BLOCK_SIZE) {
        cbo_flush(line);
        __sync_synchronize();
    }
}

void video_init() {
    struct RAMFBCfg cfg = {
        .addr = bswap64(FB_BASE),
        .fourcc = bswap32(XRGB8888),
        .flags = bswap32(0),
        .width = bswap32(FB_WIDTH),
        .height = bswap32(FB_HEIGHT),
        .stride = bswap32(FB_WIDTH * FB_BPP),
    };
    fw_cfg_write_entry(&cfg, fw_cfg_find_file("etc/ramfb"),
                       sizeof(struct RAMFBCfg));
}

void video_bmp_display(unsigned int* bmp_image, int width, int height) {
    unsigned int* fb = (unsigned int*)FB_BASE;
    int start_x = (FB_WIDTH - width) / 2;
    int start_y = (FB_HEIGHT - height) / 2;
    for (int y = 0; y < height; y++) {
        void* dst = fb + (start_y + y) * FB_WIDTH + start_x;
        memcpy(dst, bmp_image + y * width, width * sizeof(unsigned int));
        flush_dcache(dst, width * sizeof(unsigned int));
    }
}
