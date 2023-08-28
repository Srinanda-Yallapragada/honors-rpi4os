// GPIO

// Note: you might wonder why this base address differs from the one shown
// throughout the BCM2711 ARM Peripherals document. It’s because the RPi4 boots
// into Low Peripheral Mode by default. This maps the peripherals over the last
// 64mb of RAM, therefore they’re “visible to the ARM at 0x0_FEnn_nnnn”.

// https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2711/rpi_DATA_2711_1p0.pdf
//  register information resource
enum {
  PERIPHERAL_BASE = 0xFE000000,
  GPFSEL0 = PERIPHERAL_BASE + 0x200000,  // GPIO function select 0
  GPSET0 = PERIPHERAL_BASE + 0x20001C,   // GPIO Pin Output Set 0
  GPCLR0 = PERIPHERAL_BASE + 0x200028,   // GPIO Pin Output Clear 0
  GPPUPPDN0 = PERIPHERAL_BASE + 0x2000E4 // GPIO Pull-up / Pull-down Register 0.
};

enum {
  GPIO_MAX_PIN = 53,
  GPIO_FUNCTION_OUT = 1,
  GPIO_FUNCTION_ALT5 = 2,
  GPIO_FUNCTION_ALT3 = 7
};

enum {
  Pull_None = 0,
  Pull_Down = 1, // Are down and up the right way around?
  Pull_Up = 2
};

void mmio_write(long reg, unsigned int val) {
  *(volatile unsigned int *)reg = val;
} // Write directly to a register
unsigned int mmio_read(long reg) {
  return *(volatile unsigned int *)reg;
} // read directly from a register and return the value

unsigned int gpio_call(unsigned int pin_number, unsigned int value,
                       unsigned int base, unsigned int field_size,
                       unsigned int field_max) {
  unsigned int field_mask = (1 << field_size) - 1;

  if (pin_number > field_max)
    return 0;
  if (value > field_mask)
    return 0;

  unsigned int num_fields = 32 / field_size;
  unsigned int reg = base + ((pin_number / num_fields) * 4);
  unsigned int shift = (pin_number % num_fields) * field_size;

  unsigned int curval = mmio_read(reg);
  curval &= ~(field_mask << shift);
  curval |= value << shift;
  mmio_write(reg, curval);

  return 1;
}
// Functions to work with GPIO. setting and clearing GPIO

unsigned int gpio_set(unsigned int pin_number, unsigned int value) {
  return gpio_call(pin_number, value, GPSET0, 1, GPIO_MAX_PIN);
}
unsigned int gpio_clear(unsigned int pin_number, unsigned int value) {
  return gpio_call(pin_number, value, GPCLR0, 1, GPIO_MAX_PIN);
}
unsigned int gpio_pull(unsigned int pin_number, unsigned int value) {
  return gpio_call(pin_number, value, GPPUPPDN0, 2, GPIO_MAX_PIN);
}
unsigned int gpio_function(unsigned int pin_number, unsigned int value) {
  return gpio_call(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN);
}
// In our case, we want GPIO 14 and GPIO 15 to take alternate function 5 (TXD1
// and RXD1 respectively). My understanding is that we are telling the Pi to
// treat the pins as RX TX instead of GPIO's regular function
// fucntions to set alternate GPIO functions
void gpio_useAsAlt3(unsigned int pin_number) {
  gpio_pull(pin_number, Pull_None);
  gpio_function(pin_number, GPIO_FUNCTION_ALT3);
}

void gpio_useAsAlt5(unsigned int pin_number) {
  gpio_pull(pin_number, Pull_None);
  gpio_function(pin_number, GPIO_FUNCTION_ALT5);
}
// set output pin for GPIO output function
void gpio_initOutputPinWithPullNone(unsigned int pin_number) {
  gpio_pull(pin_number, Pull_None);
  gpio_function(pin_number, GPIO_FUNCTION_OUT);
}

void gpio_setPinOutputBool(unsigned int pin_number, unsigned int onOrOff) {
  if (onOrOff) {
    gpio_set(pin_number, 1);
  } else {
    gpio_clear(pin_number, 1);
  }
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

enum {
  AUX_BASE = PERIPHERAL_BASE + 0x215000,
  AUX_IRQ = AUX_BASE,
  AUX_ENABLES = AUX_BASE + 4,
  AUX_MU_IO_REG = AUX_BASE + 64,
  AUX_MU_IER_REG = AUX_BASE + 68,
  AUX_MU_IIR_REG = AUX_BASE + 72,
  AUX_MU_LCR_REG = AUX_BASE + 76,
  AUX_MU_MCR_REG = AUX_BASE + 80,
  AUX_MU_LSR_REG = AUX_BASE + 84,
  AUX_MU_MSR_REG = AUX_BASE + 88,
  AUX_MU_SCRATCH = AUX_BASE + 92,
  AUX_MU_CNTL_REG = AUX_BASE + 96,
  AUX_MU_STAT_REG = AUX_BASE + 100,
  AUX_MU_BAUD_REG = AUX_BASE + 104,
  AUX_UART_CLOCK = 500000000,
  UART_MAX_QUEUE =
      16 * 1024 // this is the maximumm uart str input that can be taken
};

// baud rate calculation formula based on cpu speed
#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK / (baud * 8)) - 1)

unsigned char uart_output_queue[UART_MAX_QUEUE];
unsigned int uart_output_queue_write =
    0; // if i understand correctly, this tracks what to write into from the RX
unsigned int uart_output_queue_read =
    0; // this tracks what to read out back to the TX

// writing the correct bits to these registers. to set up the uart correctly
void uart_init() {
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

unsigned int uart_isOutputQueueEmpty() {
  return uart_output_queue_read == uart_output_queue_write;
}

unsigned int uart_isReadByteReady() {
  return mmio_read(AUX_MU_LSR_REG) & 0x01;
} // checking status bits to see if data ready. see doccumentation for status
  // bits meaning
unsigned int uart_isWriteByteReady() {
  return mmio_read(AUX_MU_LSR_REG) &
         0x20; // cheking if transmitter idle which implies ready to write
}

unsigned char uart_readByte() {
  while (!uart_isReadByteReady()) // spinlock till ready
    ;
  return (unsigned char)mmio_read(AUX_MU_IO_REG); // read info from register
}

void uart_writeByteBlockingActual(unsigned char ch) {
  while (!uart_isWriteByteReady()) // sprintf waiting for ready
    ;
  mmio_write(AUX_MU_IO_REG, (unsigned int)ch); // write byte out to register
}

void uart_loadOutputFifo() {
  while (!uart_isOutputQueueEmpty() &&
         uart_isWriteByteReady()) { // check if there is something to write, and
                                    // you are ready to write it
    uart_writeByteBlockingActual(
        uart_output_queue[uart_output_queue_read]); // read what you have to
                                                    // write out from the
                                                    // uart_output_queue
    uart_output_queue_read =
        (uart_output_queue_read + 1) &
        (UART_MAX_QUEUE - 1); // Don't overrun, update value of read and wrap
                              // around if reach end of str
  }
}

void uart_writeByteBlocking(unsigned char ch) {
  unsigned int next =
      (uart_output_queue_write + 1) &
      (UART_MAX_QUEUE - 1); // Don't overrun, wrap around and set next

  while (next ==
         uart_output_queue_read) // queue read wont be update till everything in
                                 // the uart_output_queue has not been written
                                 // out. once that happends, it updates the
                                 // uart_output_queue_read
    uart_loadOutputFifo();       // read updates in this function

  uart_output_queue[uart_output_queue_write] = ch;
  uart_output_queue_write = next;
}

void uart_writeText(char *buffer) {
  while (*buffer) { // check end of string, write out entire text byte by byte
    if (*buffer == '\n')
      uart_writeByteBlocking('\r');    // for formatting the newline
    uart_writeByteBlocking(*buffer++); // writing out the char
  }
}

void uart_drainOutputQueue() {
  while (!uart_isOutputQueueEmpty())
    uart_loadOutputFifo(); // updates read until read = write and there is no
                           // pending data to be written out
}
