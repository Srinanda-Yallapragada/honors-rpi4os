#include "../include/kernel.h"
#include "../include/io.h"
#include "../include/string.h"

int num_timer_handle_timer_called; // global variable to keep track of how
                                   // many times the timer has gone off

// TIMER FUNCTIONS

const unsigned int timer1_int = CLOCKHZ;
unsigned int timer1_val = 0;

void timer_init()
{
    timer1_val = REGS_TIMER->counter_lo; // get the current value of the timer
    timer1_val += timer1_int;            // sets the value of the first timer interrupt to CLOCKHZ
    REGS_TIMER->compare[1] = timer1_val; // set the compare register to go off at
                                         // the value of timer1_val
}

void handle_timer_1()
{
    timer1_val += timer1_int;                      // update timer1_val to next value where interrupt
                                                   // should go off
    REGS_TIMER->compare[1] = timer1_val;           // set that to the compare register
    REGS_TIMER->control_status |= SYS_TIMER_IRQ_1; // update control status register to clear the match deteched status bit
    num_timer_handle_timer_called += 1;

    char *out_to_uart = "";
    getDecStr(out_to_uart, 3,
              num_timer_handle_timer_called);
    uart_writeText(out_to_uart); // write num_timer_handle_timer_called to uart
    uart_writeText("handle_timer_1 went off! \n");
}

void main(void)
{
    num_timer_handle_timer_called =
        0; // resetting to 0
    // it ghost sets itself up to 969 when I don't do this
    
    uart_init();
    uart_writeText("Welcome to CZPZ!\n");

    // Kick off the timers

    irq_init_vectors();
    enable_interrupt_controller();
    irq_enable();
    timer_init();

    // Loop endlessly

    while (1)
        ;
}
