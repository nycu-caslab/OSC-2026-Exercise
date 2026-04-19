# Frame Buffer

## Exercise 5 Slide
[OSC2026_lab5_exercise](<https://docs.google.com/presentation/d/1Hrk6VhzNDeHYBQ2gjyMNbeHY1GsiFy6qTeG9Uc2A8bc/edit?usp=sharing>)

## Introduction

This exercise demonstrates how to initialize and use the QEMU frame buffer device (ramfb) to display graphics and animations on a virtual display. You are required to finish the TODOs in order to be able to display the video from the frame buffer on QEMU.

- `video.c` Implements the QEMU ramfb driver for frame buffer access
- The frame buffer is mapped at address `0x87000000` with a resolution of 1920x1080.
- The animation loops through frames defined in `bird.h`, displaying each for 50ms.
- `bird.h`: Header file containing frame data for the bird animation.

## TODOs
### `main.c`
- C code that initializes video and displays an animated bird sequence.
- implement the `usleep` function to buffer the frame display each for 50ms.
- initialize the frame buffer before starting the display

## Verification

Build and run the kernel using the following command:

```bash
make run
```

This launches QEMU with the kernel image and frame buffer support, displaying the bird animation on the virtual screen.

## Expected Result
![bird](https://hackmd.io/_uploads/B1bpeOOwWl.gif)