#include "io.h"

#define MAX_INSTRUCTION_LEN 200
#define MAX_NUM_INSTRUCTIONS 10

char inst_dict[MAX_NUM_INSTRUCTIONS][MAX_INSTRUCTION_LEN] = {"cd", "ls"};

char instruction_buf[MAX_INSTRUCTION_LEN];
unsigned int instruction_buf_write_index = 0;

// This can clearly be implemented better, but the concept is that I am
// comparing char by char, the instruction dictionary and the instruction buffer
// the moment they are not equal, I return saying no command found. Only the
// first command will be parsed correcly i think. but thats for me to test
void parse_buf(char instruction_buf) {
  for (int i = 0; i < MAX_NUM_INSTRUCTIONS; i++) {
    for (int j = 0; j < MAX_INSTRUCTION_LEN; j++) {
      uart_writeText("char ");
      uart_writeText(instruction_buf[j]);
      uart_writeText("matches char");
      uart_writeText(&inst_dict[i][j]);
    }
  }
}

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
