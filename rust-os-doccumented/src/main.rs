#![no_std]
#![no_main]

use core::panic::PanicInfo;
mod io;

mod boot {
    use core::arch::global_asm;
    global_asm!(".section .text._start");
}

#[no_mangle]
pub extern "C" fn _start() -> ! {
    loop {
        unsafe {
            io::uart_write_text("hello \n");
        }
    }
}

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
