#include "../include/io.h"
#include "../include/mm.h"
#include "../include/string.h"

#define MAX_NUM_PROCESS 20
#define STACK_SIZE 512
struct process_control_block {
  unsigned long x19;
  unsigned long x20;
  unsigned long x21;
  unsigned long x22;
  unsigned long x23;
  unsigned long x24;
  unsigned long x25;
  unsigned long x26;
  unsigned long x27;
  unsigned long x28;
  unsigned long fp;
  unsigned long sp;
  unsigned long pc;
  long pid;
};

// unsigned num_processes = 42;
// struct process_control_block process_list[MAX_NUM_PROCESS] = {};

void exec(void (*f)()) { (f)(); }

void add_to_process_list(void (*f)()) {
  uart_writeText("called add to process \n");
}
