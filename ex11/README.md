# Exercise 1-1

## Intro.
This exercise marks the first step in building a custom kernel from scratch on a bare-metal machine. You will lay the essential groundwork for the operating system, focusing on defining the basic memory layout and initializing the UART interface for serial communication.

## Source Code
- `Makefile` : Automates the build process and provides commands to compile the kernel and launch it within the QEMU emulator.
- `linker.ld` : A custom linker script that explicitly defines the kernel's memory layout, dictating where sections like `.text`, `.data`, and `.bss` are placed in memory.
- `start.S` : The assembly entry point. It handles essential hardware preparatory work, such as setting up the stack pointer and clearing the BSS section, before handing control over to the C code.
- `main.c` : The core implementation of your basic kernel, containing the main execution loop and basic hardware interactions (e.g., UART communication).


## BSS Section
The `.bss` section contains uninitialized global variables, which expect a default value of `0`. Before the main kernel starts working, this entire memory region must be zeroed out.

The boundaries of this section are marked by the `__bss_start` and `__bss_stop` symbols provided by the linker script. Your job is to write an assembly loop that sets all memory addresses between these two symbols to zero.
```asm
_start:
    // TODO: Clear the .bss section
```

## Stack Pointer
The stack pointer tracks the current top of the call stack. Because the stack grows downwards in memory (from higher to lower addresses), the stack pointer must be initialized to point to the bottom of the stack (the highest memory address) before the C kernel can safely execute. In the RISC-V ABI, this is managed using the `sp` register.
```asm
_start:
    // TODO: Set up the stack pointer
```

## UART
To enable serial communication, you must implement three fundamental UART functions. We have provided several pre-defined macros that map to the memory-mapped I/O (MMIO) registers of the QEMU emulator's UART device. You will use these hardware registers to implement the following polling-based I/O functions:
- `uart_getc()` : Read and return a single character received through the UART.
- `uart_putc(char c)` : Transmit a single character through the UART.
- `uart_puts(const char* s)` : Transmit a string through the UART.
```c
char uart_getc() {
    // TODO: Implement this function
}

void uart_putc(char c) {
    // TODO: Implement this function
}

void uart_puts(const char* s) {
    // TODO: Implement this function
}
```

## Verification
We have provided a `Makefile` to automate the process of building the kernel and running it within the QEMU emulator.
```bash
make run
```
## Expected Result
You should see every character you type into the terminal immediately echoed back and displayed on the screen.
```
Starting kernel ...
hello world
hello OSC
this is uart test
```

> [!Warning]
> If your kernel compiles successfully but you encounter the following error when running QEMU:
> ```
> qemu-system-riscv64: cannot set up guest memory 'riscv_virt_board.ram': Cannot allocate memory
> ```
> This typically happens because the provided `Makefile` attempts to allocate `8 GB` of RAM for QEMU, which may exceed your host system's available memory.
> 
> Modify `Makefile` and reduce the memory allocation `-m` to `256M`
> ```Makefile
> $(QEMU) -M virt -m 256M -kernel $(TARGET) -display none -serial stdio
> ```

