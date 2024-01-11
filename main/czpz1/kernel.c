#include "kernel.h"
#include "io.h"
#include "string.h"

// from multicore.h and multicore.c
unsigned int spin_cpu1;

void store32(unsigned long address, unsigned long value) {
  *(unsigned long *)address = value;
}

unsigned long load32(unsigned long address) {
  return *(unsigned long *)address;
}
void clear_core1(void) { store32((unsigned long)&spin_cpu1, 0); }

void start_core1(void (*func)(void)) {
  store32((unsigned long)&spin_cpu1, (unsigned long)func);
  asm volatile("sev");
}

// from fb.c
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

void core0_main(void) {
  unsigned int core0_val = 0;

  while (core0_val <= 20) {
    wait_msec(0x50000);
    core0_val++;
  }

  irq_disable();
  disable_interrupt_controller();

  while (1)
    ;
}

// TIMER FUNCTIONS

const unsigned int timer1_int = CLOCKHZ;
unsigned int timer1_val = 0;

void timer_init() {
  timer1_val = REGS_TIMER->counter_lo;
  timer1_val += timer1_int;
  REGS_TIMER->compare[1] = timer1_val;
}

int num_timer_handle_timer_called = 0;

void handle_timer_1() {
  timer1_val += timer1_int;
  REGS_TIMER->compare[1] = timer1_val;
  REGS_TIMER->control_status |= SYS_TIMER_IRQ_1;
  num_timer_handle_timer_called += 1;

  char *out_to_uart = "";
  getDecStr(out_to_uart, 3,
            num_timer_handle_timer_called); // overwrites out_to_uart

  uart_writeText(out_to_uart);
  uart_writeText("handle_timer_1 went off! \n");
}

void main(void) {
  uart_init();
  uart_writeText("Is the grinding happening?!");

  // Kick off the timers

  irq_init_vectors();
  enable_interrupt_controller();
  irq_enable();
  timer_init();

  // Loop endlessly

  core0_main();
}
