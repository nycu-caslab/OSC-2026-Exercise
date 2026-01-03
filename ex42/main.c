extern char uart_getc(void);
extern void uart_putc(char c);
extern void uart_puts(const char* s);
extern void uart_hex(unsigned long h);

#define UART_BASE 0x10000000UL
#define UART_RBR  (unsigned char*)(UART_BASE + 0x0)
#define UART_THR  (unsigned char*)(UART_BASE + 0x0)
#define UART_IER  (unsigned char*)(UART_BASE + 0x1)
#define UART_IIR  (unsigned char*)(UART_BASE + 0x2)
#define UART_MCR  (unsigned char*)(UART_BASE + 0x4)
#define UART_LSR  (unsigned char*)(UART_BASE + 0x5)
#define LSR_DR    (1 << 0)
#define LSR_TDRQ  (1 << 5)
#define UART_IRQ  0x0a

#define PLIC_BASE            0xc000000UL
#define PLIC_PRIORITY(irq)   (PLIC_BASE + (irq) * 4)
#define PLIC_ENABLE(hart)    (PLIC_BASE + 0x002080 + (hart) * 0x0100)
#define PLIC_THRESHOLD(hart) (PLIC_BASE + 0x201000 + (hart) * 0x2000)
#define PLIC_CLAIM(hart)     (PLIC_BASE + 0x201004 + (hart) * 0x2000)

unsigned long boot_cpu_hartid = 0;

void uart_init() {
    // TODO: Enable RX interrupt
    // TODO: Enable UART interrupt
}

void irq_enable() {
    asm volatile("csrsi sstatus, (1 << 1)");
}

void enable_external_interrupt() {
    asm volatile(
        "li t0, (1 << 9);"
        "csrs sie, t0;");
}

void plic_init() {
    // TODO: Implement this function
    // (1) Set UART interrupt priority
    // (2) Set UART interrupt enable for the boot hart
    // (3) Set threshold for the boot hart
    // (4) Enable external interrupts
}

int plic_claim() {
    // TODO: Implement this function
}

void plic_complete(int irq) {
    // TODO: Implement this function
}

void do_trap() {
    int irq = plic_claim();
    if (irq == UART_IRQ) {
        char c = *UART_RBR;
        uart_putc(c == '\r' ? '\n' : c);
    }
    if (irq)
        plic_complete(irq);
}

void start_kernel() {
    uart_puts("\nStarting kernel ...\n");
    plic_init();
    uart_init();
    irq_enable();
    while (1)
        ;
}
