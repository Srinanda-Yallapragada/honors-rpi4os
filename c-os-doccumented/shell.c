#include "io.h"
#include "string.h"

#define MAX_INSTRUCTION_LEN 200
#define MAX_NUM_INSTRUCTIONS 10

char inst_dict[MAX_NUM_INSTRUCTIONS][MAX_INSTRUCTION_LEN] = {
    "cd",    "ls",    "touch", "delete", "test1",
    "test2", "test3", "test4", "test5",  "test6"};
char tempnumarr[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};

char instruction_buf[MAX_INSTRUCTION_LEN];
unsigned int instruction_buf_write_index = 0;

// This can clearly be implemented better, but the concept is that I am
// comparing char by char, the instruction dictionary and the instruction buffer
// the moment they are not equal, I return saying no command found. Only the
// first command will be parsed correcly i think. but thats for me to test
void parse_buf(char instruction_buf[MAX_INSTRUCTION_LEN]) {

  for (int i = 0; i < MAX_NUM_INSTRUCTIONS; i++) {
    if (cmpstr(inst_dict[i], instruction_buf) == 0) {
      uart_writeText("Parsed the instuction");
      uart_writeText(inst_dict[i]);
      uart_writeByteBlocking('\n');
    }
  }

  return;
  // for (int i = 0; i < MAX_INSTRUCTION_LEN; i++) {
  //   if (instruction_buf[i] == '\n') {
  //     return;
  //   } else {
  //     uart_writeByteBlocking(instruction_buf[i]);
  //   }
  // }

  // int inst_match = 1; // true till proven false. This probably has a few
  // issues
  //                     // that I am glazing over
  // for (int i = 0; i < MAX_NUM_INSTRUCTIONS; i++) {
  //   for (int j = 0; j < MAX_INSTRUCTION_LEN; j++) {

  //     if (inst_dict[i][j] != instruction_buf[j]) {
  //       inst_match = -1;
  //     }
  //   }
  //   if (inst_match) {
  //     uart_writeText(instruction_buf);
  //     uart_writeText("\n");
  //   }
  //   //   uart_writeText("char ");
  //   uart_writeText(instruction_buf + sizeof(char) * j);
  //   uart_writeText("matches char");
  //   uart_writeText(&inst_dict[i][j]);
  //   uart_writeText("\n");
  // }
}

void shell() {

  uart_loadOutputFifo(); // this is my understanding
  // read the initial data from the serial cable sent

  if (uart_isReadByteReady()) { // check ready to read
    unsigned char ch =
        uart_readByte(); // read the byte from the register into a char
    if (ch == '\r') {
      parse_buf(instruction_buf);
      uart_writeText("Your last instruction was: ");
      uart_writeText(instruction_buf);
      for (int i = 0; i < 200; i++) {
        instruction_buf[i] = '\0'; // Reset to 0 (null terminator)
      }
      uart_writeText("\nOS_name>"); // for visual formatting of new line
      instruction_buf_write_index = 0;
    } else {
      instruction_buf[instruction_buf_write_index] = ch;
      instruction_buf_write_index += 1;
    }
  }
}
