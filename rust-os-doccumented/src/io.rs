pub static PERIPHERAL_BASE: u64 = 0xFE000000;

pub static GPFSEL0: u64 = PERIPHERAL_BASE + 0x200000; // GPIO function select 0
pub static GPSET0: u64 = PERIPHERAL_BASE + 0x20001C; // GPIO Pin Output Set 0
pub static GPCLR0: u64 = PERIPHERAL_BASE + 0x200028; // GPIO Pin Output Clear 0
pub static GPPUPPDN0: u64 = PERIPHERAL_BASE + 0x2000E4; // GPIO Pull-up / Pull-down Register 0.

pub enum GpioFuctionSelector {
    GpioMaxPin = 53,
    GpioFunctionOut = 1,
    GpioFunctionAlt5 = 2,
    GpioFunctionAlt3 = 7,
}

pub enum GpioPull {
    PullNone = 0,
    PullDown = 1,
    PullUp = 2,
}

// This function exists to write values directly to
// physical memory locations to set GPIO pins and statu flags and such
pub unsafe fn mmio_write(register_address: u64, value: u64) {
    core::ptr::write_volatile(register_address as *mut u64, value);
}

pub unsafe fn mmio_read(register_address: u64) -> u64 {
    return core::ptr::read_volatile(register_address as *const u64);
}

pub unsafe fn gpio_call(
    pin_number: u64,
    value: u64,
    base: u64,
    field_size: u64,
    field_max: u64,
) -> u64 {
    let field_mask = (1 << field_size) - 1;
    if pin_number > field_max {
        return 0;
    } else if value > field_mask {
        return 0;
    } else {
        let num_fields: u64 = 32 / field_size;
        let reg: u64 = base + ((pin_number / num_fields) * 4);
        let shift: u64 = (pin_number % num_fields) * field_size;

        let mut curval: u64 = mmio_read(reg);
        curval &= !(field_mask << shift);
        curval |= value << shift;
        mmio_write(reg, curval);

        return 1;
    }
}

pub unsafe fn gpio_set(pin_number: u64, value: u64) -> u64 {
    return gpio_call(
        pin_number,
        value,
        GPSET0,
        1,
        GpioFuctionSelector::GpioMaxPin as u64,
    );
}

pub unsafe fn gpio_clear(pin_number: u64, value: u64) -> u64 {
    return gpio_call(
        pin_number,
        value,
        GPCLR0,
        1,
        GpioFuctionSelector::GpioMaxPin as u64,
    );
}

pub unsafe fn gpio_pull(pin_number: u64, value: u64) -> u64 {
    return gpio_call(
        pin_number,
        value,
        GPPUPPDN0,
        2,
        GpioFuctionSelector::GpioMaxPin as u64,
    );
}

pub unsafe fn gpio_function(pin_number: u64, value: u64) -> u64 {
    return gpio_call(
        pin_number,
        value,
        GPFSEL0,
        3,
        GpioFuctionSelector::GpioMaxPin as u64,
    );
}

pub unsafe fn gpio_use_as_alt3(pin_number: u64) {
    gpio_pull(pin_number, GpioPull::PullNone as u64);
    gpio_function(pin_number, GpioFuctionSelector::GpioFunctionAlt3 as u64);
}

pub unsafe fn gpio_use_as_alt5(pin_number: u64) {
    gpio_pull(pin_number, GpioPull::PullNone as u64);
    gpio_function(pin_number, GpioFuctionSelector::GpioFunctionAlt5 as u64);
}
pub unsafe fn gpio_init_output_pin_with_pull_none(pin_number: u64) {
    gpio_pull(pin_number, GpioPull::PullNone as u64);
    gpio_function(pin_number, GpioFuctionSelector::GpioFunctionOut as u64);
}

pub unsafe fn gpio_set_pin_output_bool(pin_number: u64, on_or_off: bool) {
    if on_or_off {
        gpio_set(pin_number, 1);
    } else {
        gpio_clear(pin_number, 1);
    }
}

pub static AUX_BASE: u64 = PERIPHERAL_BASE + 0x215000;
pub static AUX_IRQ: u64 = AUX_BASE;
pub static AUX_ENABLES: u64 = AUX_BASE + 4;
pub static AUX_MU_IO_REG: u64 = AUX_BASE + 64;
pub static AUX_MU_IER_REG: u64 = AUX_BASE + 68;
pub static AUX_MU_IIR_REG: u64 = AUX_BASE + 72;
pub static AUX_MU_LCR_REG: u64 = AUX_BASE + 76;
pub static AUX_MU_MCR_REG: u64 = AUX_BASE + 80;
pub static AUX_MU_LSR_REG: u64 = AUX_BASE + 84;
pub static AUX_MU_MSR_REG: u64 = AUX_BASE + 88;
pub static AUX_MU_SCRATCH: u64 = AUX_BASE + 92;
pub static AUX_MU_CNTL_REG: u64 = AUX_BASE + 96;
pub static AUX_MU_STAT_REG: u64 = AUX_BASE + 100;
pub static AUX_MU_BAUD_REG: u64 = AUX_BASE + 104;
pub static AUX_UART_CLOCK: u64 = 500000000;
pub static UART_MAX_QUEUE: u64 = 16 * 1024; // this is the maximumm uart str input that can be taken

pub fn aux_mu_baud_calc(baud: u64) -> u64 {
    return (AUX_UART_CLOCK / (baud * 8)) - 1;
}

pub static mut UART_OUTPUT_QUEUE: [char; 16 * 1024] = ['\0'; (16 * 1024)];

//writing to a static mut is a bad idea in rust because of race issues, need to wrap any access to
//this stuff in unsafe brackets see example below
// unsafe fn test() {
//     UART_OUTPUT_QUEUE[54] = 'h';
// }
//
//
pub static mut UART_OUTPUT_QUEUE_WRITE: u64 = 0;

pub static mut UART_OUTPUT_QUEUE_READ: u64 = 0;

pub unsafe fn uart_init() {
    mmio_write(AUX_ENABLES, 1); // enable UART1
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_CNTL_REG, 0);
    mmio_write(AUX_MU_LCR_REG, 3); // 8 bits
    mmio_write(AUX_MU_MCR_REG, 0);
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_IIR_REG, 0xC6); // disable interrupts
    mmio_write(AUX_MU_BAUD_REG, aux_mu_baud_calc(115200));
    gpio_use_as_alt5(14);
    gpio_use_as_alt5(15);
    mmio_write(AUX_MU_CNTL_REG, 3); // enable RX/TX
}

// pub unsafe fn uart_is_output_queue_empty() -> bool {
//     return UART_OUTPUT_QUEUE_WRITE == UART_OUTPUT_QUEUE_READ;
// }

pub unsafe fn uart_is_write_byte_ready() -> u64 {
    return mmio_read(AUX_MU_LSR_REG) & 0x20;
}

pub unsafe fn uart_write_by_blocking_actual(char: char) {
    while uart_is_write_byte_ready() > 0 {}
    mmio_write(AUX_MU_IO_REG, char as u64);
}

pub unsafe fn uart_write_text(message: &str) {
    for c in message.chars() {
        if c == '\n' {
            uart_write_by_blocking_actual('\r')
        };
        uart_write_by_blocking_actual(c);
    }
}
