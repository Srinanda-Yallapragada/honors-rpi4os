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

unsigned num_processes = 0;
struct process_control_block process_list[MAX_NUM_PROCESS] = {};

void exec(void (*f)()) { (f)(); }

void add_to_process_list(unsigned long fn, unsigned long arg) {
  // add num process check to prevent adding to many processese
  num_processes += 1;
  struct process_control_block *p;

  p = (struct process_control_block *)get_free_page();
  p->pid = num_processes;
  p->x19 = fn;
}

void run_programs() {}
