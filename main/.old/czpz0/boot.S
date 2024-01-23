.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:
    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1  // mpidr_el1 is a special register that is essentially a core identification mechanism 
    and     x1, x1, #3   //#3 is 0b11 meaning x1 will only be 0 if the main core with core id 0 is the core running this assmebly code
    cbz     x1, 2f //2f is tricky notation, essentially meaning find the 2: label forward from this instruction. CBZ -> Compare and branch on 0
    // if x1 is 0, we are on the main core

    // We're not on the main core, so hang in an infinite wait loop
1:  wfe //wait-for-event
    b       1b


2:  // We're on the main core!

    // Set stack to start below our code
    ldr     x1, =_start // load register with start address
    mov     sp, x1 //sp is stack pointer so you are poitning the stack pointer to the start address

    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address //BSS is block starting symbol according to wikipedia
    ldr     w2, =__bss_size      // Size of the section
3:  cbz     w2, 4f               // Quit loop if zero
    str     xzr, [x1], #8        // assigning 0 to the entire stack. xzr is the 0 register
    sub     w2, w2, #1           // going over all the addresses in the stack one at a time
    cbnz    w2, 3b               // Loop if non-zero

    // Jump to our main() routine in C (make sure it doesn't return)
4:  bl      main
    // In case it does return, halt the master core too
    b       1b
