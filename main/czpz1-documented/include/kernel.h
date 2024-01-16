#define PERIPHERAL_BASE 0xFE000000
#define CLOCKHZ 1000000

struct timer_regs {
    volatile unsigned int control_status; // System Timer Control/Status
    volatile unsigned int counter_lo; // Low 32 bits
    volatile unsigned int counter_hi; // High 32 bits
    volatile unsigned int compare[4]; // System Timer Compare. 4 lines of compare.
};

#define REGS_TIMER ((struct timer_regs *)(PERIPHERAL_BASE + 0x00003000))  //REGS_TIMER points to system timer registers

struct arm_irq_regs_2711 {
    volatile unsigned int irq0_pending_0; // Shows the status of Enabled interrupts  
    volatile unsigned int irq0_pending_1;
    volatile unsigned int irq0_pending_2;
    volatile unsigned int res0; // reserved bits 
    volatile unsigned int irq0_enable_0;
    volatile unsigned int irq0_enable_1;
    volatile unsigned int irq0_enable_2;
    volatile unsigned int res1; // reserved bits
    volatile unsigned int irq0_disable_0;
    volatile unsigned int irq0_disable_1;
    volatile unsigned int irq0_disable_2;
};

typedef struct arm_irq_regs_2711 arm_irq_regs;

#define REGS_IRQ ((arm_irq_regs *)(PERIPHERAL_BASE + 0x0000B200)) // REGS_IRQ points to irq registers 

enum vc_irqs {
    SYS_TIMER_IRQ_0 = 1, // relevant bits for different timer interrupts
    SYS_TIMER_IRQ_1 = 2,
    SYS_TIMER_IRQ_2 = 4,
    SYS_TIMER_IRQ_3 = 8,
    AUX_IRQ = (1 << 29)
};

void irq_init_vectors();
void irq_enable();
void irq_disable();
void enable_interrupt_controller(); 
void disable_interrupt_controller();

void handle_timer_1();
