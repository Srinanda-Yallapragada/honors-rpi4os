#include "../include/io.h"
#include "../include/irqentry.h"
#include "../include/mm.h"
#include "../include/string.h"

#define MAX_NUM_PROCESS 20
#define THREAD_SIZE 4096
unsigned num_processes = 0;
struct process_control_block process_list[MAX_NUM_PROCESS];
unsigned cur_process_pid = 0;

// void exec(unsigned int f) {
//   cpu_run(f);
// }

void add_to_process_list(unsigned long fn, unsigned long arg)
{
  // add num process check to prevent adding to many processese
  struct process_control_block *p;

  p = (struct process_control_block *)get_free_page();
  p->pid = num_processes;
  p->x19 = fn;
  p->x20 = arg;
  p->pc = (unsigned long)ret_from_fork;
  p->sp = (unsigned long)p + THREAD_SIZE;
  // uart out the x19 and x20
  char *output_to_uart = "";
  getDecStr(output_to_uart, 10, p->x19);
  uart_writeText(output_to_uart);
  uart_writeText("\n");
  process_list[num_processes] = *p;
  getDecStr(output_to_uart, 10, process_list[num_processes].x19);
  uart_writeText(output_to_uart);
  uart_writeText("\n");
  num_processes += 1;
}

void schedule_and_run()
{
  // load from process list[0] and then execture
  // load information from process list[0]
  // then execute at the end the function
  uart_writeText("Scheduling and running\n");
  char *output_to_uart = "";
  // print out process list[0] information
  getDecStr(output_to_uart, 10, process_list[0].x19);
  uart_writeText(output_to_uart);
  uart_writeText("\n");
  getDecStr(output_to_uart, 10, process_list[0].x20);
  uart_writeText(output_to_uart);
  uart_writeText("\n");

  cpu_run(process_list[0].x19, process_list[0].x20);
}
