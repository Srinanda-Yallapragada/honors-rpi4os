#include "../include/sysregs.h"

#define ARM_CONTROL   0xff800000 //Main Timer and AXI Error Control.
#define TIMER_INCREMENT 0xff800008 //1 = increment count by two. 0 = increment count by one. 
#define OSC_FREQ        54000000
#define MAIN_STACK      0x400000

.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:

    ldr     x0, =ARM_CONTROL   // Main timer control register   
    str     wzr, [x0]
    mov     w1, 0x80000000
    str     w1, [x0, #(TIMER_INCREMENT - ARM_CONTROL)] // This is code that assumed c preprocessor
    // ldr     x5, =TIMER_INCREMENT // this is the alternative for rust
    // ldr     x6, =ARM_CONTROL
    // sub     x5, x5, x6
    // str     w1, [x0, x5]

    ldr     x0, =OSC_FREQ // set up oscillation frequency for the timer 
    msr     cntfrq_el0, x0 //TODO
    msr     cntvoff_el2, xzr //TODO
    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    // We're not on the main core, so hang in an infinite wait loop
1:  wfe
    b       1b // Loop infinite
2:  // We're on the main core!
    // First enable the FPU

    mov     x0, #0x33ff
    msr     cptr_el3, x0 	 // Disable coprocessor traps to EL3
    mov     x0, #3 << 20
    msr     cpacr_el1, x0	 // Enable FP/SIMD at EL1

    // Now get ready to switch from EL3 down to EL1
    // Signals CPU to execute in EL1 by setting particular bits
    // These are predefined flags/ registers
    ldr     x0, =SCTLR_VALUE_MMU_DISABLED
    msr	    sctlr_el1, x0		

    ldr     x0, =HCR_VALUE
    msr     hcr_el2, x0

    ldr     x0, =SCR_VALUE
    msr     scr_el3, x0

    ldr     x0, =SPSR_VALUE
    msr     spsr_el3, x0
    
    adr     x0, el1_entry		
    msr     elr_el3, x0

    eret			
el1_entry:
    // We're in EL1

    // Set stack to start below our code
    ldr     x1, =_start 
    mov     sp, x1

    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section
3:  cbz     w2, 4f               // Quit loop if zero
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b               // Loop if non-zero

    // Jump to our main() routine in C (make sure it doesn't return)
4:  bl      main
    // In case it does return, halt the master core too
    b       1b

.ltorg
