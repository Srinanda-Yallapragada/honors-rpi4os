#![no_std]
#![no_main]

use core::arch::{asm, global_asm};
use core::panic::PanicInfo;
mod io;

use io::uart_init;

// Include the bootloader when Linking
global_asm!(include_str!("boot.S"));

mod boot {
    use core::arch::global_asm;
    global_asm!(".section .text._start");
}

#[no_mangle]
pub extern "C" fn _start() -> ! {
    uart_init();
    loop {
        io::uart_writeText("hello \n");
    }
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
