.section ".text.boot"
.global _start
_start:
    // TODO: Clear the .bss section
    // TODO: Set up the stack pointer
    tail start_kernel
