#include "kernel.h"
#include "../include/fb.h"
#include "../include/io.h"
#include "../include/multicore.h"
#include "../include/string.h"

int counter = 0; // global counter

void core0_main(void) {
  char *out_to_uart = "";
  while (counter <= 50) {
    wait_msec(0x50000);
    uart_writeText("Core 0:");
    getDecStr(out_to_uart, 3, counter); // overwrites out_to_uart
    uart_writeText(out_to_uart);
    counter++;
  } // exits loop after 50 seconds

  irq_disable();
  disable_interrupt_controller();

  while (1)
    ;
}

void core1_main(void) {

  clear_core1(); // Only run once

  wait_msec(0x25000);
  char *out_to_uart = "";
  while (counter <= 50) {
    wait_msec(0x50000);
    uart_writeText("Core 1:");
    getDecStr(out_to_uart, 3, counter); // overwrites out_to_uart
    uart_writeText(out_to_uart);
    counter++;
  } // exits loop after 50 seconds

  while (1)
    ;
}

// TIMER FUNCTIONS

const unsigned int timer1_int = CLOCKHZ;
const unsigned int timer3_int = CLOCKHZ / 4;
unsigned int timer1_val = 0;
unsigned int timer3_val = 0;

void timer_init() {
  timer1_val = REGS_TIMER->counter_lo;
  timer1_val += timer1_int;
  REGS_TIMER->compare[1] = timer1_val;

  timer3_val = REGS_TIMER->counter_lo;
  timer3_val += timer3_int;
  REGS_TIMER->compare[3] = timer3_val;
}

void handle_timer_1() {
  timer1_val += timer1_int;
  REGS_TIMER->compare[1] = timer1_val;
  REGS_TIMER->control_status |= SYS_TIMER_IRQ_1;

  unsigned int progval = timer1_val / timer1_int;
  if (progval <= 50) {
    uart_writeText("interrupt 1 went off ");
  } 
}

void handle_timer_3() {
  timer3_val += timer3_int;
  REGS_TIMER->compare[3] = timer3_val;
  REGS_TIMER->control_status |= SYS_TIMER_IRQ_3;

  unsigned int progval = timer3_val / timer3_int;
   if (progval <= 50)
    uart_writeText("interrupt 3 went off ");
;
}

void main(void) {
  uart_init();

  unsigned int i = 0;
  while (i++ < 30)
    debugcrlf();

  // Kick it off on core 1

  start_core1(core1_main);

  // Kick off the timers

  irq_init_vectors();
  enable_interrupt_controller();
  irq_enable();
  timer_init();

  // Loop endlessly

  core0_main();
}
