#include "../include/kernel.h"
#include "../include/io.h"
#include "../include/mm.h"
#include "../include/process.h"
#include "../include/string.h"

int num_timer_handle_timer_called = 0;

void wait_msec(unsigned int n) {
  register unsigned long f, t, r;

  // Get the current counter frequency
  asm volatile("mrs %0, cntfrq_el0" : "=r"(f));
  // Read the current counter
  asm volatile("mrs %0, cntpct_el0" : "=r"(t));
  // Calculate expire value for counter
  t += ((f / 1000) * n) / 1000;
  do {
    asm volatile("mrs %0, cntpct_el0" : "=r"(r));
  } while (r < t);
}

// testing exec fuction of a fake workload essentially
void sayMoo() { uart_writeText("\n Moo! \n "); }

void sayMeow() { uart_writeText("\n Meow! \n "); }

void spell_5_letter_word(char *array) {
  while (1) {
    for (int i = 0; i < 5; i++) {
      uart_writeText(&array[i]);
      wait_msec(100000);
    }
  }
}

// TIMER FUNCTIONS

const unsigned int timer1_int = CLOCKHZ;
unsigned int timer1_val = 0;

void timer_init() {
  timer1_val = REGS_TIMER->counter_lo;
  timer1_val += timer1_int;
  REGS_TIMER->compare[1] = timer1_val;
}

void handle_timer_1() {
  timer1_val += timer1_int;
  REGS_TIMER->compare[1] = timer1_val;
  REGS_TIMER->control_status |= SYS_TIMER_IRQ_1;
  num_timer_handle_timer_called += 1;

  // char *out_to_uart = "";
  // getDecStr(out_to_uart, 3,
  //           num_timer_handle_timer_called); // overwrites out_to_uart

  // uart_writeText(out_to_uart);
  uart_writeText("handle_timer_1 went off! \n");
}

void main(void) {
  uart_init();
  uart_writeText("switcheroooo?!");

  add_to_process_list((unsigned long)&spell_5_letter_word,
                      (unsigned long)"1234");
  add_to_process_list((unsigned long)&spell_5_letter_word,
                      (unsigned long)"abcd");

  // Kick off the timers

  irq_init_vectors();
  enable_interrupt_controller();
  irq_enable();
  timer_init();

  num_timer_handle_timer_called =
      0; // resseting here as you want to see this after the timer has been
         // initted normally, about 650 interrupts go off before we see output
         // on the uart wihtout this line

  // Loop endlessly

  while (1)
    ;
}
