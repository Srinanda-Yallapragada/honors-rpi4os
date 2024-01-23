#include "io.h"
#include "string.h"

#define MAX_INSTRUCTION_LEN 200
#define MAX_NUM_INSTRUCTIONS 10


// programs that are being included from their source files
//  assumes that the uart has been initialized
//  prog_info meaning program_info following naming convention of prog_{program
//  name that is typed into console} example if you want to create a command
//  called abc, then make a file called prog_abc.c and prog_abc.h and fill those
//  in with you functionality
#include "prog_info.h"

//  assumes that the uart has been initialized
int exec(char instruction[MAX_INSTRUCTION_LEN]) {
  uart_writeText("executing instruction ");
  uart_writeText(instruction);
  uart_writeText("\n");

  if (cmpstr(instruction, "info") == 0) {
    uart_writeText("made it here!");
    info(); // calling info function from prog_info.
  }

  return 1;
}
