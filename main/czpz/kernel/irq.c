#include "../include/kernel.h"

void enable_interrupt_controller() {
  REGS_IRQ->irq0_enable_0 = SYS_TIMER_IRQ_1; // sets the irq0_enable_0 to 1 enabling the 0th timer interrupt
}

void disable_interrupt_controller() { REGS_IRQ->irq0_enable_0 = 0; } //disable irq0_enable_0

void handle_irq() {
  unsigned int irq = REGS_IRQ->irq0_pending_0; //reads pending interrupts for irq0 channel

  while (irq) { // while there are pending interrupts as an interrupt may go off while dealing with the current interrupt
    if (irq & SYS_TIMER_IRQ_1) { // if the irq is the 0th timer interrupt
      irq &= ~SYS_TIMER_IRQ_1; // clear the interrupt

      handle_timer_1();// handle the timer interrupt
    }
  }
}
