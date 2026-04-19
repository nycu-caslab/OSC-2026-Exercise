# Thread Creation

## Exercise 5 Slide
[OSC2026_lab5_exercise](<https://docs.google.com/presentation/d/1Hrk6VhzNDeHYBQ2gjyMNbeHY1GsiFy6qTeG9Uc2A8bc/edit?usp=sharing>)

## Introduction

In this exercise, you will learn how to perform context switch between processes with a simple scheduler built with a queue.

A `task_struct` structure stores vital data about a process, including the register states, PID, and pointers to other related processes. For lab5, more attributes may be expected from this structure.

## Context Switch
In this exercise, a context switch saves process's state(registers, program counter, stack pointer) and loads the saved state of the next process.

## TODOs

###  `start.S`
- Assembly code responsible for booting the kernel, handling exceptions
- Implement the `switch_to` function for context switching.
### `main.c`
- C code that defines the task structure, manages the run queue, and contains the scheduling logic.
- Implement the `schedule` function that selects the next task to run from the run queue and do context switch.


## Verification

Build and run the kernel using the following command:

```bash
make run
```

## Expected Result
```bash
Starting kernel ...
Process ID: 0x0000000000000003 0x0000000000000000
Process ID: 0x0000000000000002 0x0000000000000000
Process ID: 0x0000000000000001 0x0000000000000000
Process ID: 0x0000000000000003 0x0000000000000001
Process ID: 0x0000000000000002 0x0000000000000001
Process ID: 0x0000000000000001 0x0000000000000001
Process ID: 0x0000000000000003 0x0000000000000002
Process ID: 0x0000000000000002 0x0000000000000002
Process ID: 0x0000000000000001 0x0000000000000002
Process ID: 0x0000000000000003 0x0000000000000003
Process ID: 0x0000000000000002 0x0000000000000003
Process ID: 0x0000000000000001 0x0000000000000003
Process ID: 0x0000000000000003 0x0000000000000004
Process ID: 0x0000000000000002 0x0000000000000004
Process ID: 0x0000000000000001 0x0000000000000004
```