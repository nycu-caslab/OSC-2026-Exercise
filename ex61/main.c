extern char uart_getc(void);
extern void uart_putc(char c);
extern void uart_puts(const char* s);

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

#define SATP_MODE_SV39 (8UL << 60)

unsigned long __attribute__((section(".data"), aligned(PAGE_SIZE))) pg_dir[512];

void setup_vm() {
    // TODO: Implement this function
}

void start_kernel() {
    uart_puts("\nStarting kernel ...\n");
    while (1) {
        uart_putc(uart_getc());
    }
}
