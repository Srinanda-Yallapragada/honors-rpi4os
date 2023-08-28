#include "io.h"

#define MAX_INSTRUCTION_LEN 200

char instruction_buf[MAX_INSTRUCTION_LEN];
unsigned int instruction_buf_write_index = 0;
void shell() {

  uart_loadOutputFifo(); // this is my understanding
  // read the initial data from the serial cable sent
  //

  if (uart_isReadByteReady()) { // check ready to read
    unsigned char ch =
        uart_readByte(); // read the byte from the register into a char
    if (ch == '\r') {
      uart_writeText("Your last instruction was: ");
      uart_writeText(instruction_buf);
      uart_writeText("\nOS_name>"); // for visual formatting of new line
      instruction_buf_write_index = 0;
    } else {
      instruction_buf[instruction_buf_write_index] = ch;
      instruction_buf_write_index += 1;
    }
  }
}
