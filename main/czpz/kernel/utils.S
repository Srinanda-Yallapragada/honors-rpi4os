.globl irq_init_vectors // loads vector table into vbar_el1
irq_init_vectors:
    adr x0, vectors 
    msr vbar_el1, x0
    ret

.globl irq_enable // enable interrupts
irq_enable:
    msr daifclr, #2 //debug, abort (SError), IRQ, and FIQ
    ret

.globl irq_disable // disable interrupts
irq_disable:
    msr daifset, #2 //debug, abort (SError), IRQ, and FIQ
    ret
