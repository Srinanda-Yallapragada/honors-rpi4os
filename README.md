# Microkernel Development: Rust vs C on Raspberry Pi 4

## Project Overview

This repository is for my Honors Portfolio @ UMass Amherst. It explores bare-metal programming on the RPi4, aiming to highlight the developer experience of both C and Rust. The primary objective was to provide valuable insights for developers, particularly those venturing into the intricacies of low-level coding in Rust coming from a background in C.


## Folder Structure

- **main/czpz-commented:**
  - Contains the C implementation of the single-core interrupt firing microkernel.

  - **Files:**
    - `main/czpz/boot/`
      - `boot.S`: ARM assembly instructions for timer configuration, EL1 setup, stack establishment, and main kernel entry point. Initially taken from rpi4os.com.
      - `link.ld`: Linker script used during compilation. Initially taken from rpi4os.com.
      
    - `main/czpz/include`
      - `io.h`: Header file with UART function declarations. Initially taken from rpi4os.com.
      - `kernel.h`: Contains structs defining timer and interrupt registers, along with IRQ-related function headers. Initially taken from rpi4os.com.
      - `string.h`: Implements basic string functions (e.g., lenstr, cmpstr)
      - `sysregs.h`: Includes registers and bit flags for transitioning into EL1 during the boot process. Initially taken from rpi4os.com.

    - `main/czpz/kernel`
      - `irq.c`: Manages enabling, disabling, and handling of interrupt requests. Initially taken from rpi4os.com.
      - `irqentry.S`: Assembly code for loading the interrupt vector table and managing system state during interrupts. Initially taken from rpi4os.com.
      - `kernel.c`: Main kernel file where the system initializes the timer, sets up interrupts, and enters an endless loop.
      - `utils.S`: Assembly file with helper functions for vector table initialization, IRQ enabling, and IRQ disabling. Initially taken from rpi4os.com.

    - `main/czpz/lib`
      - `io.c`: Implementation of UART functions. Initially taken from rpi4os.com.
      - `string.c`: Implementation of string functions.

- **main/rhd2-commented:**
  - Contains the Rust implementation of the microkernel.
  - **Files:**
    - `main/rhd2/`
      - `.cargo/config.toml`: Configuration file instructing the compiler to use the correct linker script and triple target aarch64-unknown-none-softfloat.
      - `Cargo.toml`:Describes project metadata and dependencies.
      - `Cargo.lock`: Automatically generated file describing dependencies based on Cargo.toml.
      - `linker.ld`: Linker script used during compilation. Initially taken from rpi4os.com.

    - `main/rhd2/src`
      - `boot.S`: ARM assembly instructions for timer configuration, EL1 setup, stack establishment, and main kernel entry point. Initially taken from rpi4os.com.
      - `io.rs`: Rust file containing io.h declarations and their implementations, consolidating UART-related functionality.
      - `irqentry.S`: Assembly code for loading the interrupt vector table and managing system state during interrupts. Initially taken from rpi4os.com.
      - `irq.rs`: Contains structs defining timer and interrupt registers, along with IRQ-related function headers. Manages enabling, disabling, and handling of interrupt requests.
      - `main.rs`: Main kernel file where the system initializes the timer, sets up interrupts, and enters an endless loop.
      - `utils.S`: Assembly file with helper functions for vector table initialization, IRQ enabling, and IRQ disabling. Initially taken from rpi4os.com.
      

## Getting Started

To build and run the microkernels, follow these steps:

### C Microkernel

1. Navigate to the `/c_microkernel` directory.
2. Run `make` to build the C microkernel.
3. Load the compiled binary onto the Raspberry Pi 4.

### Rust Microkernel

1. Navigate to the `/rust_microkernel` directory.
2. Run `cargo build` to build the Rust microkernel.
3. Load the compiled binary onto the Raspberry Pi 4.

## Conclusion

The project sheds light on the nuances of bare-metal programming in Rust and C. Despite challenges, Rust's user-friendly error messages and tooling demonstrate its potential advantages. The GitHub repository includes detailed documentation and code comparisons for further exploration.

Feel free to explore the code and contribute to the project!
