#include "exec.h"
#include "io.h"
#include "string.h"

#define MAX_INSTRUCTION_LEN 200
#define MAX_NUM_INSTRUCTIONS 10

char inst_dict[MAX_NUM_INSTRUCTIONS][MAX_INSTRUCTION_LEN] = {
    "cd",    "ls",    "touch", "delete", "info",
    "test1", "test2", "test3", "test4",  "test5"};

char instruction_buf[MAX_INSTRUCTION_LEN];
unsigned int instruction_buf_write_index = 0;

// This can clearly be implemented better, but the concept is that I am
// comparing char by char, the instruction dictionary and the instruction buffer
// the moment they are not equal, I return saying no command found. Only the
// first command will be parsed correcly i think. but thats for me to test


int is_valid_instruction(char instruction_buf[MAX_INSTRUCTION_LEN]) {
  for (int i = 0; i < MAX_NUM_INSTRUCTIONS; i++) {
    if (cmpstr(inst_dict[i], instruction_buf) == 0) {
      uart_writeText("Parsed the instruction\n");
      uart_writeText(inst_dict[i]);
      uart_writeByteBlocking('\n');
      return 1; // Valid instruction
    }
  }
  return 0; // Invalid instruction
}

void shell() {

  uart_loadOutputFifo(); // this is my understanding
  // read the initial data from the serial cable sent

  if (uart_isReadByteReady()) { // check ready to read
    unsigned char ch =
        uart_readByte(); // read the byte from the register into a char

    if (ch == '\r') { // enter was hit by the user

      if (is_valid_instruction(instruction_buf)) {
        uart_writeText("Executing ");
        uart_writeText(instruction_buf);
        uart_writeText("\n");
        exec(instruction_buf);
      } else {
        uart_writeText("Your last instruction was not recognized");
      }

      for (int i = 0; i < MAX_INSTRUCTION_LEN; i++) {
        instruction_buf[i] = '\0'; // Reset to 0 (null terminator)
      }

      uart_writeText("\nOS_name>");    // for visual formatting of new line
      instruction_buf_write_index = 0; // reset write location of the buffer
    } else {
      instruction_buf[instruction_buf_write_index] =
          ch; // reading in characters till '\r' is found
      instruction_buf_write_index += 1;
    }
  }
}
