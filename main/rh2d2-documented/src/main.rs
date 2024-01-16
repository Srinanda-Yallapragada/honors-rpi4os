#![no_std]
#![no_main]
use core::arch::global_asm;
use core::panic::PanicInfo;
// buffer[i] = (digit as u8) + b'0';
// Include the bootloader when Linking
global_asm!(include_str!("boot.S"));
global_asm!(include_str!("utils.S"));
global_asm!(include_str!("irqentry.S"));

// written using the help of copilot
pub fn convert_int_to_str(number: u32) {
    let mut num: u32 = number;
    let mut buffer = [0u8; 10];
    let mut i = 0;
    let str_arr = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"];
    while num > 0 {
        let digit: usize = (num % 10) as usize;
        num /= 10;
        buffer[i] = digit as u8;
        i += 1;
    }
    for digit in buffer[0..i].iter().rev() {
        uart_writeText(str_arr[*digit as usize]);
    }
    uart_writeText("\n");
}

//find the assembly functions
extern "C" {
    fn irq_enable();
    fn irq_init_vectors();
}

mod io;
use crate::io::*;

mod irq;
use crate::irq::*;

// infinite loop to prevent the kernel from returning when panic occurs
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

// static mut is considred bad code! but I do not know how to do this in a better way
static mut TIMER_VAL: u32 = 0;
static mut NUM_TIMER_CALLED: u32 = 0;

#[no_mangle]
pub fn timer_init() -> () {
    // set the compare register to 1000000 clock cycles in the future
    unsafe {
        TIMER_VAL = core::ptr::read_volatile(TIMER_REGS.counter_lo); // read the current value of the timer
        TIMER_VAL += TIMER_INT; // add 1000000 to the timer for compare to trigger interrupt
        core::ptr::write_volatile(TIMER_REGS.compare[1] as *mut u32, TIMER_VAL);
        //set compare register
    }
}
#[no_mangle]
pub fn handle_timer_1() -> () {
    unsafe {
        NUM_TIMER_CALLED += 1; // static mut is being updated
        TIMER_VAL += TIMER_INT;
        core::ptr::write_volatile(TIMER_REGS.compare[1] as *mut u32, TIMER_VAL); //set compare register
        let cur_control_status = core::ptr::read_volatile(TIMER_REGS.control_status); // read the current status of the timer
        core::ptr::write_volatile(
            TIMER_REGS.control_status as *mut u32,
            cur_control_status | SYS_TIMER_IRQ_1,
        ); // clear the interrupt in the control status register
    }
    unsafe {
        convert_int_to_str(NUM_TIMER_CALLED);
    }
    uart_writeText("handle_timer_1() went off!\n");
}

#[no_mangle]
#[link_section = ".text._start"]

pub extern "C" fn main() -> ! { // assembly jump to this function from boot.S
    //no_mangle is used to prevent the compiler from mangling the name of the function
    //link_section is used to tell the linker where to put the function in the binary
    uart_init();
    uart_writeText("Welcome to RH2D!\n");

    //These functions are called in utils.S, assembly code is obviously unsafe.
    unsafe {
        irq_init_vectors();
        enable_interrupt_controller();
        irq_enable();
        timer_init();

        NUM_TIMER_CALLED = 0; // if this line is not present, our counter starts at ~67108866 
        // in c that was only ~970
    }

    // kernel should not return

    loop {}
}
