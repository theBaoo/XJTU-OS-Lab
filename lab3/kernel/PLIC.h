#define PLIC_BASE 0x0c000000
#define PLIC_PRIORITY    (PLIC_BASE + 0x0)
#define PLIC_PENDING     (PLIC_BASE + 0x1000)
#define PLIC_SENABLE(hart) (PLIC_BASE + 0x2080 + (hart) * 0x100)
#define PLIC_SPRIORITY(hart) (PLIC_BASE + 0x201000 + (hart) * 0x2000)
#define PLIC_THRESHOLD(hart) (PLIC_BASE + 0x200000 + (hart) * 0x1000)
#define PLIC_SCLAIM(hart)     (PLIC_BASE + 0x201004 + (hart) * 0x2000)

#define UART0_IRQ 10

void plic_set_priority(int irq, int priority);
void plic_enable(int hart, int irq);
void plic_set_threshold(int hart, int threshold);
int plic_claim(int hart);
void plic_complete(int hart, int irq);
void external_interrupt_handler();
