use crate::handle_timer_1;

pub const PERIPHERAL_BASE: u32 = 0xFE000000;
pub const CLOCKHZ: u32 = 1000000;

pub const SYS_TIMER_IRQ_0: u32 = 1;
pub const SYS_TIMER_IRQ_1: u32 = 2;
pub const SYS_TIMER_IRQ_2: u32 = 4;
pub const SYS_TIMER_IRQ_3: u32 = 8;
pub const AUX_IRQ: u32 = 1 << 29;

pub struct TimerRegsBCM2711 {
    pub control_status: *const u32,
    pub counter_lo: *const u32,
    pub counter_hi: *const u32,
    pub compare: [*const u32; 4],
}
// see page 142 https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf
pub const TIMER_REGS: TimerRegsBCM2711 = TimerRegsBCM2711 {
    control_status: (PERIPHERAL_BASE + 0x00003000) as *const u32,
    counter_lo: (PERIPHERAL_BASE + 0x00003004) as *const u32,
    counter_hi: (PERIPHERAL_BASE + 0x00003008) as *const u32,
    compare: [
        (PERIPHERAL_BASE + 0x0000300C) as *const u32,
        (PERIPHERAL_BASE + 0x00003010) as *const u32,
        (PERIPHERAL_BASE + 0x00003014) as *const u32,
        (PERIPHERAL_BASE + 0x00003018) as *const u32,
    ],
};

// Not using res0 here as in this version, I am writing the memory addresses directly
pub struct ArmIrqRegs2711 {
    pub irq0_pending_0: *const u32,
    pub irq0_pending_1: *const u32,
    pub irq0_pending_2: *const u32,
    pub irq0_enable_0: *const u32,
    pub irq0_enable_1: *const u32,
    pub irq0_enable_2: *const u32,
    pub irq0_disable_0: *const u32,
    pub irq0_disable_1: *const u32,
    pub irq0_disable_2: *const u32,
}

// see intterupst page 100 for addresses https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf
pub const ARM_IRQ_REGS: ArmIrqRegs2711 = ArmIrqRegs2711 {
    irq0_pending_0: (PERIPHERAL_BASE + 0x0000B200) as *const u32,
    irq0_pending_1: (PERIPHERAL_BASE + 0x0000B204) as *const u32,
    irq0_pending_2: (PERIPHERAL_BASE + 0x0000B208) as *const u32,
    irq0_enable_0: (PERIPHERAL_BASE + 0x0000B210) as *const u32,
    irq0_enable_1: (PERIPHERAL_BASE + 0x0000B214) as *const u32,
    irq0_enable_2: (PERIPHERAL_BASE + 0x0000B218) as *const u32,
    irq0_disable_0: (PERIPHERAL_BASE + 0x0000B220) as *const u32,
    irq0_disable_1: (PERIPHERAL_BASE + 0x0000B224) as *const u32,
    irq0_disable_2: (PERIPHERAL_BASE + 0x0000B228) as *const u32,
};

#[no_mangle]
pub fn enable_interrupt_controller() -> () {
    unsafe {
        // enable the system timer interrupt IRQ 1
        core::ptr::write_volatile(ARM_IRQ_REGS.irq0_enable_0 as *mut u32, SYS_TIMER_IRQ_1);
    }
}
#[no_mangle]
pub fn disable_interrupt_controller() -> () {
    unsafe {
        // disable the system timer interrupt IRQ 1
        core::ptr::write_volatile(ARM_IRQ_REGS.irq0_enable_0 as *mut u32, 0);
    }
}

#[no_mangle]
pub fn handle_irq() {
    unsafe {
        let mut irq: u32 = core::ptr::read_volatile(ARM_IRQ_REGS.irq0_pending_0);

        while irq != 0 { // while there are pending interrupts
            if (irq & SYS_TIMER_IRQ_1 ) != 0 { //if the irq is the 0th timer interrupt 
                irq &= !SYS_TIMER_IRQ_1; // clear the interrupt
                handle_timer_1(); // call the timer handler
            }
        }
    }
}
