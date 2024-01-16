// GPIO

enum
{ // gpio register addresses
    PERIPHERAL_BASE = 0xFE000000,
    GPFSEL0 = PERIPHERAL_BASE + 0x200000,
    GPSET0 = PERIPHERAL_BASE + 0x20001C,
    GPCLR0 = PERIPHERAL_BASE + 0x200028,
    GPPUPPDN0 = PERIPHERAL_BASE + 0x2000E4
};

enum
{ // gpio function select
    GPIO_MAX_PIN = 53,
    GPIO_FUNCTION_ALT5 = 2,
};

enum
{ // gpio pull up/down
    Pull_None = 0,
};

void mmio_write(long reg, unsigned int val) { *(volatile unsigned int *)reg = val; } // volatile write to address
unsigned int mmio_read(long reg) { return *(volatile unsigned int *)reg; }           // volatile read from address

// gpio_call writes to the gpio registers
unsigned int gpio_call(unsigned int pin_number, unsigned int value, unsigned int base, unsigned int field_size, unsigned int field_max)
{
    unsigned int field_mask = (1 << field_size) - 1;

    if (pin_number > field_max) // check if pin number is out of range
        return 0;
    if (value > field_mask) // check if value is out of range
        return 0;

    unsigned int num_fields = 32 / field_size;                   // number of fields per register
    unsigned int reg = base + ((pin_number / num_fields) * 4);   // get the register address
    unsigned int shift = (pin_number % num_fields) * field_size; // get the shift amount

    unsigned int curval = mmio_read(reg); // read the current value
    curval &= ~(field_mask << shift);     // clear the field
    curval |= value << shift;             // set the field
    mmio_write(reg, curval);              // write the new value

    return 1;
}
// gpio set and clear are wrappers for gpio_call

unsigned int gpio_set(unsigned int pin_number, unsigned int value) { return gpio_call(pin_number, value, GPSET0, 1, GPIO_MAX_PIN); }
unsigned int gpio_clear(unsigned int pin_number, unsigned int value) { return gpio_call(pin_number, value, GPCLR0, 1, GPIO_MAX_PIN); }
unsigned int gpio_pull(unsigned int pin_number, unsigned int value) { return gpio_call(pin_number, value, GPPUPPDN0, 2, GPIO_MAX_PIN); }
unsigned int gpio_function(unsigned int pin_number, unsigned int value) { return gpio_call(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN); }

// In our case, we want GPIO 14 and GPIO 15 to take alternate function 5 (TXD1
// and RXD1 respectively). My understanding is that we are telling the Pi to
// treat the pins as RX TX instead of GPIO's regular function
// fucntions to set alternate GPIO functions
void gpio_useAsAlt5(unsigned int pin_number)
{
    gpio_pull(pin_number, Pull_None);
    gpio_function(pin_number, GPIO_FUNCTION_ALT5);
}

// UART
// https://grasslab.github.io/osdi/en/hardware/uart.html
// This resources has a lot of information on the enums below. It explains all
// the registers. You are working with physical addresses so no security here
// https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf This has
// the exact information about these registers. Basically used to set up the
// miniuart with interrupts and other setup configuration.
//
// Theses are only the address, we set the bits in uart_init()

enum
{
    AUX_BASE = PERIPHERAL_BASE + 0x215000,
    AUX_ENABLES = AUX_BASE + 4,
    AUX_MU_IO_REG = AUX_BASE + 64,
    AUX_MU_IER_REG = AUX_BASE + 68,
    AUX_MU_IIR_REG = AUX_BASE + 72,
    AUX_MU_LCR_REG = AUX_BASE + 76,
    AUX_MU_MCR_REG = AUX_BASE + 80,
    AUX_MU_LSR_REG = AUX_BASE + 84,
    AUX_MU_CNTL_REG = AUX_BASE + 96,
    AUX_MU_BAUD_REG = AUX_BASE + 104,
    AUX_UART_CLOCK = 500000000,
    UART_MAX_QUEUE = 16 * 1024
};
// baud rate calculation formula based on cpu speed
#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK / (baud * 8)) - 1)

void uart_init()
{
    mmio_write(AUX_ENABLES, 1); // enable UART1
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_CNTL_REG, 0);
    mmio_write(AUX_MU_LCR_REG, 3); // 8 bits
    mmio_write(AUX_MU_MCR_REG, 0);
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_IIR_REG, 0xC6); // disable interrupts
    mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));
    gpio_useAsAlt5(14);
    gpio_useAsAlt5(15);
    mmio_write(AUX_MU_CNTL_REG, 3); // enable RX/TX
}

// cheking if transmitter idle which implies ready to write
unsigned int uart_isWriteByteReady() { return mmio_read(AUX_MU_LSR_REG) & 0x20; }

void uart_writeByteBlockingActual(unsigned char ch)
{
    while (!uart_isWriteByteReady())
        ;
    mmio_write(AUX_MU_IO_REG, (unsigned int)ch); // write byte out to register
}

void uart_writeText(char *buffer)
{
    while (*buffer) // check end of string, write out entire text byte by byte
    {
        if (*buffer == '\n')
            uart_writeByteBlockingActual('\r');  // for formatting the newline
        uart_writeByteBlockingActual(*buffer++); // writing out the char and incrementing the pointer
    }
}
