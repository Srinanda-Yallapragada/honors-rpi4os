#![no_std]
#![no_main]
use core::arch;
use core::panic::PanicInfo;

use core::arch::global_asm;

// Include the bootloader when Linking
global_asm!(include_str!("boot.S"));

mod io;
use crate::io::*;

#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    loop {}
}

// https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf
const START: usize = 0xFE00_0000; // Based on section 1.2 of manual
const GPIO_OFFSET: usize = 0x0020_0000; // Based on section 5.2 of manual, also check that
                                        // 0x7enn_nnnn is mapped to 0xFEnn_nnnn
const GPIO_START: usize = START + GPIO_OFFSET;

#[no_mangle]
#[link_section = ".text._start"]
pub extern "C" fn main() -> ! {
    // this needs to be called main  because start is defined in the
    // boot.S file
    uart_init();
    uart_writeText("Started\n");

    unsafe {
        // Set GPIO 42 with GPFSEL4[8:6] = 0b001
        core::ptr::write_volatile((GPIO_START + 0x10) as *mut u32, 0b001 << 6);
        loop {
            // Set GPIO 42 to HIGH with GPSET1
            core::ptr::write_volatile((GPIO_START + 0x20) as *mut u32, 1 << (42 - 32));

            // Wait
            for _ in 0..500000 {
                arch::asm!("nop");
            }

            // Set GPIO 42 to LOW with GPCLR[42-32]
            core::ptr::write_volatile((GPIO_START + 0x02c) as *mut u32, 1 << (42 - 32));

            // Wait
            for _ in 0..500000 {
                arch::asm!("nop");
            }
        }
    }
}
