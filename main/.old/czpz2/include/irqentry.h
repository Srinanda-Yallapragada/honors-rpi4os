#ifndef _ENTRY_H
#define _ENTRY_H

#define S_FRAME_SIZE 272 // size of all saved registers

#define SYNC_INVALID_EL1t 0
#define IRQ_INVALID_EL1t 1
#define FIQ_INVALID_EL1t 2
#define ERROR_INVALID_EL1t 3

#define SYNC_INVALID_EL1h 4
#define IRQ_INVALID_EL1h 5
#define FIQ_INVALID_EL1h 6
#define ERROR_INVALID_EL1h 7

#define SYNC_INVALID_EL0_64 8
#define IRQ_INVALID_EL0_64 9
#define FIQ_INVALID_EL0_64 10
#define ERROR_INVALID_EL0_64 11

#define SYNC_INVALID_EL0_32 12
#define IRQ_INVALID_EL0_32 13
#define FIQ_INVALID_EL0_32 14
#define ERROR_INVALID_EL0_32 15
struct process_control_block {
  unsigned long x19;
  unsigned long x20;
  unsigned long x21;
  unsigned long x22;
  unsigned long x23;
  unsigned long x24;
  unsigned long x25;
  unsigned long x26;
  unsigned long x27;
  unsigned long x28;
  unsigned long fp;
  unsigned long sp;
  unsigned long pc;
  long pid;
};

#ifndef __ASSEMBLER__

void ret_from_fork(void);

void cpu_run(unsigned int function_addr, unsigned int arg);

void cpu_switch_to(struct process_control_block *prev,
                   struct process_control_block *next);

#endif
#endif
