.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:
    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1 //Move to ARM register from system coprocessor register. https://developer.arm.com/documentation/ddi0595/2021-12/AArch64-Registers/MPIDR-EL1--Multiprocessor-Affinity-Register?lang=en. Unsure what the purpose of the regiser is. see docs
    and     x1, x1, #3
    cbz     x1, 2f //Compare and Branch on Zero
    // We're not on the main core, so hang in an infinite wait loop
1:  wfe //wait for event
    b       1b // branch to 
2:  // We're on the main core!

    // Set stack to start below our code
    ldr     x1, =_start //load register
    mov     sp, x1 //move 

    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section
3:  cbz     w2, 4f               // Quit loop if zero
    str     xzr, [x1], #8 //store
    sub     w2, w2, #1 //subtract
    cbnz    w2, 3b               // Loop if non-zero //compare and branch if not zer 

    // Jump to our main() routine in C (make sure it doesn't return)
4:  bl      main //branch with link
    // In case it does return, halt the master core too
    b       1b
