use core::ptr; //useful to write strings directly to register

//See https://datasheets.raspberrypi.com/bcm2835/bcm2835-peripherals.pdf

const PERIPHERAL_BASE: u32 = 0xFE00_0000; //base for all the peripherals
const GPFSEL0: u32 = PERIPHERAL_BASE + 0x200000; // GPIO Function selector 0
const GPSET0: u32 = PERIPHERAL_BASE + 0x20001C; // GPIO  Pin Output Set 0
const GPCLR0: u32 = PERIPHERAL_BASE + 0x200028; // GPIO Pin Output Clear 0
const GPPUPPDN0: u32 = PERIPHERAL_BASE + 0x2000E4; // GPIO Pull up Pull down
const MAX_PIN: u32 = 53;
const FUNCTION_ALT5: u32 = 2;
const Pull_None: u32 = 0;

// see https://datasheets.raspberrypi.com/bcm2835/bcm2835-peripherals.pdf page 8 for a table with all the registers

const AUX_BASE: u32 = PERIPHERAL_BASE + 0x215000;
const AUX_ENABLES: u32 = AUX_BASE + 4;
const AUX_MU_IO_REG: u32 = AUX_BASE + 64;
const AUX_MU_IER_REG: u32 = AUX_BASE + 68;
const AUX_MU_IIR_REG: u32 = AUX_BASE + 72;
const AUX_MU_LCR_REG: u32 = AUX_BASE + 76;
const AUX_MU_MCR_REG: u32 = AUX_BASE + 80;
const AUX_MU_LSR_REG: u32 = AUX_BASE + 84;
const AUX_MU_CNTL_REG: u32 = AUX_BASE + 96;
const AUX_MU_STAT_REG: u32 = AUX_BASE + 100;
const AUX_MU_BAUD_REG: u32 = AUX_BASE + 104;
const AUX_UART_CLOCK: u32 = 500_000_000; // We set the RaspberryPi Config.txt to 500 minimum core freq.
const UART_MAX_QUEUE: u32 = 16 * 1024;

fn mmio_read(reg: u32) -> u32 {
    // registers at 32 bit
    unsafe {
        // first unsafe code. it is unsafe as you reading from a main memory address
        return ptr::read_volatile(reg as *mut u32);
    }
}

fn mmio_write(reg: u32, val: u32) {
    //registers are 32 bit so you can only write 32 bit data to
    //a 32 bit addr
    // unsafe as you are writing to a main memory address
    unsafe { return ptr::write_volatile(reg as *mut u32, val as u32) }
}




// this fuction strongly mimics the C tutorial at rpi4os.com
fn gpio_call(pin_number: u32, value: u32, base: u32, field_size: u32, field_max: u32) -> u32 {
    // Create a mask to tell what Register we are using
    // Max Pin is 53, so that is our field_size
    //  1 << 0x03
    let field_mask: u32 = (1 << field_size) - 1;

    if pin_number > field_max {
        return 0;
    }
    if value > field_mask {
        return 0;
    }

    let num_fields: u32 = 32 / field_size;
    let reg: u32 = base + ((pin_number / num_fields) * 4);
    let shift: u32 = (pin_number % num_fields) * field_size;

    let mut curval: u32 = mmio_read(reg) as u32;
    curval &= !(field_mask << shift);
    curval |= value << shift;
    mmio_write(reg, curval);

    return 1;
}

fn gpio_set(pin_number: u32, value: u32) -> u32 {
    return gpio_call(pin_number, value, GPSET0 as u32, 1, MAX_PIN as u32);
}

fn gpio_clear(pin_number: u32, value: u32) -> u32 {
    return gpio_call(pin_number, value, GPCLR0 as u32, 1, MAX_PIN as u32);
}

fn gpio_pull(pin_number: u32, value: u32) -> u32 {
    return gpio_call(pin_number, value, GPPUPPDN0 as u32, 2, MAX_PIN as u32);
}

fn gpio_function(pin_number: u32, value: u32) -> u32 {
    return gpio_call(pin_number, value, GPFSEL0 as u32, 3, MAX_PIN as u32);
}

fn gpio_useAsAlt5(pin_number: u32) {
    gpio_pull(pin_number, Pull_None as u32);
    gpio_function(pin_number, FUNCTION_ALT5 as u32);
}

pub fn AUX_MU_BAUD(baud: u32) -> u32 {
    return (AUX_UART_CLOCK as u32 / (baud * 8)) - 1;
}

pub fn uart_init() {
    // MMIO WRITE to Register UART ENABLES STARTS THE COMMUNICATION WHEN BIT 0 = 1
    mmio_write(AUX_ENABLES as u32, 1);
    // WRITE TO REGISTER INTERRUPT ENABLE, BIT 0 ENABLES INTERRUPT FOR TX and RX
    mmio_write(AUX_MU_IER_REG as u32, 0);
    // WRITE TO REGISTER TO CONTROL THE UART FLOW, DONT RECEIVE OR SEND ANYTHING IN PROGRESS
    mmio_write(AUX_MU_CNTL_REG as u32, 0);
    // SET UART IN 8 BIT MODE
    mmio_write(AUX_MU_LCR_REG as u32, 3);
    // UART_RTS line is HIGH = 0
    mmio_write(AUX_MU_MCR_REG as u32, 0);
    // Again set Interrupt Enable to Enabled?
    mmio_write(AUX_MU_IER_REG as u32, 0);
    // <Not possible> Interrupts
    mmio_write(AUX_MU_IIR_REG as u32, 0xC6);
    // Set Baud Rate using a Function AUX MU BAUD
    mmio_write(AUX_MU_BAUD_REG as u32, AUX_MU_BAUD(115200));
    // Set the UART Pins to TX and RX respectively
    // ALT5 is the function of these pins for TX and RX
    gpio_useAsAlt5(14);
    gpio_useAsAlt5(15);

    // WRITE TO REGISTER TO CONTROL THE UART FLOW, ALLOW TO RECEIVE OR SEND ANYTHING IN PROGRESS
    mmio_write(AUX_MU_CNTL_REG as u32, 3);
}

fn uart_isWriteByteReady() -> u32 {
    // Read the Data Status. If '0010_0000' is read, then FIFO can accept at least one byte
    return mmio_read(AUX_MU_LSR_REG as u32) & 0x20;
}

fn uart_writeByteBlockingActual(ch: char) {
    // Check if the UART is ready to send
    while uart_isWriteByteReady() == 0 {}
    // Send if good
    mmio_write(AUX_MU_IO_REG as u32, ch as u32);
}

pub fn uart_writeText(buffer: &str) {
    // Grab buffer String and make it an iterable type.
    // This will allow to iterate over the string
    // Without having to use the address of the string character.
    let mut x = buffer.chars().fuse(); // if none is returned by an iterator it will keep returning
    // none appapretnly // see https://www.reddit.com/r/rust/comments/sbdb9t/i_finally_understand_the_naming_of_iteratorfuse/
    // essentially meaning it will stop once you run out of chars in the buffer

    // Some type C, will be a char type. When *c = &x gets read, it will go to the next character
    while let Some(c) = x.next() {
        if c == '\n' {
            uart_writeByteBlockingActual('\n');
        } else {
            uart_writeByteBlockingActual(c);
        }
    }
}

