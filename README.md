# Microkernel Development: Rust vs C on Raspberry Pi 4

## Project Overview

This repository is for my Honors Portfolio @ UMass Amherst. It explores bare-metal programming on the RPi4, aiming to highlight the developer experience of both C and Rust. The primary objective was to provide valuable insights for developers, particularly those venturing into the intricacies of low-level coding in Rust coming from a background in C. Heavily following the rpi4os.com tutorial on writing a baremetal OS for the Raspberry Pi 4B. See the write up [here.](Srinanda_Thesis.pdf)


## Folder Structure

- **main/czpz-commented:**
  - Contains the C implementation of a single-core interrupt firing microkernel.

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
  - Contains the Rust implementation of a single-core interrupt firing microkernel.
  - **Files:**
    - `main/rhd2/`
      - `.cargo/config.toml`: Configuration file instructing the compiler to use the correct linker script and triple target aarch64-unknown-none-softfloat.
      - `Cargo.toml`:Describes project metadata and dependencies.
      - `Cargo.lock`: Automatically generated file describing dependencies based on Cargo.toml.
      - `linker.ld`: Linker script used during compilation. Initially taken from rpi4os.com.

    - `main/rhd2/src/`
      - `boot.S`: ARM assembly instructions for timer configuration, EL1 setup, stack establishment, and main kernel entry point. Initially taken from rpi4os.com.
      - `io.rs`: Rust file containing io.h declarations and their implementations, consolidating UART-related functionality.
      - `irqentry.S`: Assembly code for loading the interrupt vector table and managing system state during interrupts. Initially taken from rpi4os.com.
      - `irq.rs`: Contains structs defining timer and interrupt registers, along with IRQ-related function headers. Manages enabling, disabling, and handling of interrupt requests.
      - `main.rs`: Main kernel file where the system initializes the timer, sets up interrupts, and enters an endless loop.
      - `utils.S`: Assembly file with helper functions for vector table initialization, IRQ enabling, and IRQ disabling. Initially taken from rpi4os.com.
      

## Getting Started

To build and run the microkernels, follow these steps:

### C Microkernel

1. Download the Arm GNU Toolchain. I used version arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-elf for this project.
2. Change the Makefile path to where you installed the Arm GNU toolchain. 
3. Run `make` to build the C microkernel.
4. Copy the kernel8.img generated onto your Raspberry Pi 4B. Make sure it also contains the following files: bcm2711-rpi-4-b.dtb, fixup4.dat, start4.elf, config.txt.
5. Connect your Raspberry Pi 4B through GPIO pins to your system using a USB to TTL Serial Cable. Use the rp4 file for configuration of Putty, where you will see the output.


### Rust Microkernel

1. Install rust using rustup. I installed the language using the defaults.
2. Install the required target by running `rustup target add aarch64-unknown-none-softfloat thumbv7m-none-eabi` and `rustup component add llvm-tools-preview`
3. Run `cargo build` to build the Rust microkernel.
4. Run `cargo objcopy --release -- -O binary kernel8.img` to generate the kernel binary.
5. Copy the kernel8.img generated onto your Raspberry Pi 4B. Make sure it also contains the following files: bcm2711-rpi-4-b.dtb, fixup4.dat, start4.elf, config.txt.
6. Connect your Raspberry Pi 4B through GPIO pins to your system using a USB to TTL Serial Cable. Use the rp4 file for configuration of Putty, where you will see the output.


The project sheds light on the beginner process of bare-metal programming in Rust and C. Rust's memory safey design and user-friendly error messages demonstrate its potential advantages in the systems space over C. The GitHub repository includes commented code in main and my unfinsihed progress in exploratory-code where I tried working to create an exec function, a shell and implement context switching in the C verion.
