#![no_std]
#![no_main]
use core::panic::PanicInfo;
use core::arch::global_asm;

// Include the bootloader when Linking
global_asm!(include_str!("boot.S"));
global_asm!(include_str!("utils.S"));
global_asm!(include_str!("irqentry.S"));

extern "C" {
    fn irq_enable();
    fn irq_init_vectors();
}

mod io;
use crate::io::*;

mod irq;
use crate::irq::*;

#[panic_handler]
fn panic(_panic: &PanicInfo<'_>) -> ! {
    loop {}
}

// tried to use this but failed to understad how it works
// use core::cell::UnsafeCell;
//
// static TIMER_VAL: UnsafeCell<u32> = UnsafeCell::new(0);
// static NUM_TIMER_CALLED: UnsafeCell<u32> = UnsafeCell::new(0);

// This is not the rustic way! but code that runs is better than code that doesnt run but is pristinely written!

const TIMER_INT: u32 = 1000000;
static mut TIMER_VAL: u32 = 0;
static mut NUM_TIMER_CALLED: u32 = 0;
#[no_mangle]

pub fn timer_init() -> () {
    // set the compare register to 1 second in the future
    unsafe {
        TIMER_VAL = core::ptr::read_volatile(TIMER_REGS.counter_lo);
        TIMER_VAL += TIMER_INT;
        core::ptr::write_volatile(TIMER_REGS.compare[1] as *mut u32, TIMER_VAL);
    }
}
#[no_mangle]

pub fn handle_timer_1() -> () {
    unsafe {
        NUM_TIMER_CALLED += 1;

        TIMER_VAL += TIMER_INT;
        core::ptr::write_volatile(TIMER_REGS.compare[1] as *mut u32, TIMER_VAL);
        let cur_control_status = core::ptr::read_volatile(TIMER_REGS.control_status);
        core::ptr::write_volatile(
            TIMER_REGS.control_status as *mut u32,
            cur_control_status | SYS_TIMER_IRQ_1,
        );
    }
    uart_writeText("handle_timer_1() went off!\n");
}

#[no_mangle]
#[link_section = ".text._start"]

pub extern "C" fn main() -> ! {
    // this needs to be called main  because start is defined in the
    // boot.S file
    uart_init();
    uart_writeText("Zooted\n");

    //These functions are called in utils.S, meaning assembly code is obviously unsafe.
    unsafe {
        irq_init_vectors();

        uart_writeText("irq_init_vectors didnt fail\n");
        enable_interrupt_controller();
        //check the memroy address and verify that it did set those bits 


        uart_writeText("enable_interrupt_controller didnt fail\n");
        irq_enable();

        uart_writeText("irq_enable didnt fail\n");
        timer_init();

        uart_writeText("timer_init didnt fail\n");
        NUM_TIMER_CALLED = 0;

    }

    // kernel should not return

    loop {}
}
