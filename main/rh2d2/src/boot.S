// #include "../include/sysregs.h" this line assumed c preprocessor, but i had to modify to suit rust


// ***************************************
// SCTLR_EL1, System Control Register (EL1), Page 2654 of AArch64-Reference-Manual.
// ***************************************

.equ SCTLR_RESERVED             ,     (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
.equ SCTLR_EE_LITTLE_ENDIAN     ,     (0 << 25)
.equ SCTLR_EOE_LITTLE_ENDIAN    ,     (0 << 24)
.equ SCTLR_I_CACHE_DISABLED     ,     (0 << 12)
.equ SCTLR_D_CACHE_DISABLED     ,     (0 << 2)
.equ SCTLR_I_CACHE_ENABLED      ,     (1 << 12)
.equ SCTLR_D_CACHE_ENABLED      ,     (1 << 2)
.equ SCTLR_MMU_DISABLED         ,     (0 << 0)
.equ SCTLR_MMU_ENABLED          ,     (1 << 0)

.equ SCTLR_VALUE_MMU_DISABLED,	(SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_ENABLED | SCTLR_D_CACHE_ENABLED | SCTLR_MMU_DISABLED)

// ***************************************
// HCR_EL2, Hypervisor Configuration Register (EL2), Page 2487 of AArch64-Reference-Manual.
// ***************************************

.equ HCR_RW	   , 			(1 << 31)
.equ HCR_VALUE,			HCR_RW

// ***************************************
// SCR_EL3, Secure Configuration Register (EL3), Page 2648 of AArch64-Reference-Manual.
// ***************************************

.equ SCR_RESERVED,	    		(3 << 4)
.equ SCR_RW		,		(1 << 10)
.equ SCR_NS		,		(1 << 0)
.equ SCR_VALUE	 ,   	    	(SCR_RESERVED | SCR_RW | SCR_NS)

// ***************************************
// SPSR_EL3, Saved Program Status Register (EL3) Page 389 of AArch64-Reference-Manual.
// ***************************************

.equ SPSR_MASK_ALL, 			(7 << 6)
.equ SPSR_EL1h	,		(5 << 0)
.equ SPSR_VALUE	,		(SPSR_MASK_ALL | SPSR_EL1h)


.equ LOCAL_CONTROL   ,0xff800000
.equ LOCAL_PRESCALER ,0xff800008
.equ OSC_FREQ        ,54000000
.equ MAIN_STACK      ,0x400000


.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:

    ldr     x0, =LOCAL_CONTROL   // Sort out the timer
    str     wzr, [x0]
    mov     w1, 0x80000000
    // str     w1, [x0, #(LOCAL_PRESCALER - LOCAL_CONTROL)] This is code that assumed c preprocessor
    ldr     x5, =LOCAL_PRESCALER // this is the alternative for rust
    ldr     x6, =LOCAL_CONTROL
    sub     x5, x5, x6
    str     w1, [x0, x5]

    ldr     x0, =OSC_FREQ
    msr     cntfrq_el0, x0
    msr     cntvoff_el2, xzr
    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    // We're not on the main core, so hang in an infinite wait loop
1:  wfe
    b       1b
2:  // We're on the main core!
    // First enable the FPU

    mov     x0, #0x33ff
    msr     cptr_el3, x0 	 // Disable coprocessor traps to EL3
    mov     x0, #3 << 20
    msr     cpacr_el1, x0	 // Enable FP/SIMD at EL1

    // Now get ready to switch from EL3 down to EL1

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
