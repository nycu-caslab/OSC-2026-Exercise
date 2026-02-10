extern char uart_getc(void);
extern void uart_putc(char c);
extern void uart_puts(const char* s);
extern void uart_hex(unsigned long h);
extern int hextoi(const char* s, int n);
extern int align(int n, int byte);
extern int memcmp(const void* s1, const void* s2, int n);
extern void* memcpy(void* dst, const void* src, int n);
extern void* memset(void* s, int c, int n);
extern void* alloc_page();

#define NUM_PAGES 0x280000

#define PAGE_OFFSET 0xffffffc000000000UL
#define PAGE_SIZE   (1UL << 12)
#define HPAGE_SIZE  (1UL << 30)
#define PFN_DOWN(x) ((x) >> 12)

/* Page protection bits */
#define PAGE_PRESENT  (1 << 0)
#define PAGE_READ     (1 << 1)
#define PAGE_WRITE    (1 << 2)
#define PAGE_EXEC     (1 << 3)
#define PAGE_USER     (1 << 4)
#define PAGE_GLOBAL   (1 << 5)
#define PAGE_ACCESSED (1 << 6)
#define PAGE_DIRTY    (1 << 7)
#define PAGE_SOFT     (3 << 8)
#define PAGE_KERNEL                                                    \
    (PAGE_PRESENT | PAGE_READ | PAGE_WRITE | PAGE_EXEC | PAGE_GLOBAL | \
     PAGE_ACCESSED | PAGE_DIRTY)
#define PAGE_BASE      (PAGE_DIRTY | PAGE_ACCESSED | PAGE_USER | PAGE_PRESENT)
#define PAGE_RX        (PAGE_BASE | PAGE_READ | PAGE_EXEC)
#define PAGE_RW        (PAGE_BASE | PAGE_READ | PAGE_WRITE)
#define SATP_MODE_SV39 (8UL << 60)

#define virt_to_phys(x) ((unsigned long)(x) - PAGE_OFFSET)
#define phys_to_virt(x) ((unsigned long)(x) + PAGE_OFFSET)

unsigned long __attribute__((section(".data"), aligned(PAGE_SIZE))) pg_dir[512];

void setup_vm() {
    for (int i = 0; i < NUM_PAGES / (HPAGE_SIZE / PAGE_SIZE); i++) {
        pg_dir[256 + i] = (i * (HPAGE_SIZE / PAGE_SIZE)) << 10 | PAGE_KERNEL;
    }
    asm("csrw satp, %0" ::"r"(PFN_DOWN((unsigned long)pg_dir) |
                              SATP_MODE_SV39));
    asm("sfence.vma");
}

static void pagewalk(unsigned long va, unsigned long pa, unsigned long prot) {
    // TODO: Implement this function
}

void map_pages(unsigned long va,
               unsigned long size,
               unsigned long pa,
               unsigned long prot) {
    for (int i = 0; i < size; i += PAGE_SIZE)
        pagewalk(va + i, pa + i, prot);
}

// TODO:
#define INITRD_BASE phys_to_virt(0xa0200000)

struct cpio_t {
    char magic[6];
    char ino[8];
    char mode[8];
    char uid[8];
    char gid[8];
    char nlink[8];
    char mtime[8];
    char filesize[8];
    char devmajor[8];
    char devminor[8];
    char rdevmajor[8];
    char rdevminor[8];
    char namesize[8];
    char check[8];
};

int exec(const char* filename) {
    char* p = (char*)INITRD_BASE;
    while (memcmp(p + sizeof(struct cpio_t), "TRAILER!!!", 10)) {
        struct cpio_t* hdr = (struct cpio_t*)p;
        int namesize = hextoi(hdr->namesize, 8);
        int filesize = hextoi(hdr->filesize, 8);
        int headsize = align(sizeof(struct cpio_t) + namesize, 4);
        int datasize = align(filesize, 4);
        if (!memcmp(p + sizeof(struct cpio_t), filename, namesize)) {
            void* program = alloc_page();  // The test program fits in one page
            memcpy(program, p + headsize, filesize);
            map_pages(0x0, filesize, virt_to_phys(program), PAGE_RX);
            map_pages(0x3ffffff000, PAGE_SIZE, virt_to_phys(alloc_page()),
                      PAGE_RW);
            asm volatile("csrw sepc, %0" : : "r"(0x0));
            asm volatile("csrw sscratch, sp");
            asm volatile("mv sp, %0" ::"r"(0x4000000000));
            asm volatile(
                "li t0, (1 << 8);"
                "csrc sstatus, t0;");
            asm volatile("sret");
        }
        p += headsize + datasize;
    }
    return -1;
}

void start_kernel() {
    uart_puts("\nStarting kernel ...\n");
    if (exec("prog.bin"))
        uart_puts("Failed to exec user program!\n");
    while (1) {
        uart_putc(uart_getc());
    }
}

struct pt_regs {
    unsigned long ra;
    unsigned long sp;
    unsigned long gp;
    unsigned long tp;
    unsigned long t0;
    unsigned long t1;
    unsigned long t2;
    unsigned long s0;
    unsigned long s1;
    unsigned long a0;
    unsigned long a1;
    unsigned long a2;
    unsigned long a3;
    unsigned long a4;
    unsigned long a5;
    unsigned long a6;
    unsigned long a7;
    unsigned long s2;
    unsigned long s3;
    unsigned long s4;
    unsigned long s5;
    unsigned long s6;
    unsigned long s7;
    unsigned long s8;
    unsigned long s9;
    unsigned long s10;
    unsigned long s11;
    unsigned long t3;
    unsigned long t4;
    unsigned long t5;
    unsigned long t6;
    unsigned long epc;
    unsigned long status;
    unsigned long cause;
    unsigned long badaddr;
};

void do_trap(struct pt_regs* regs) {
    uart_puts("sepc: ");
    uart_hex(regs->epc);
    uart_puts(", scause: ");
    uart_hex(regs->cause);
    uart_puts("\n");
    regs->epc += 4;
}
