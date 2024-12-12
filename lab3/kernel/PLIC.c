#include "types.h"
#include "riscv.h"
#include "PLIC.h"

void plic_set_priority(int irq, int priority) {
    volatile uint32 *priority_reg = (uint32 *)(PLIC_PRIORITY + irq * 4);
    *priority_reg = priority;
}

void plic_enable(int hart, int irq) {
    volatile uint32 *enable_reg = (uint32 *)(PLIC_SENABLE(hart));
    *enable_reg |= (1 << irq);
}

// 0 is safe. 7 is the highest priority.
void plic_set_threshold(int hart, int threshold) {
    volatile uint32 *threshold_reg = (uint32 *)(PLIC_THRESHOLD(hart));
    *threshold_reg = threshold;
}

int plic_claim(int hart) {
    volatile uint32 *claim_reg = (uint32 *)(PLIC_SCLAIM(hart));
    return *claim_reg;
}

void plic_complete(int hart, int irq) {
    volatile uint32 *complete_reg = (uint32 *)(PLIC_SCLAIM(hart));
    *complete_reg = irq;
}

void external_interrupt_handler() {
    int irq = plic_claim(0);
    if (irq == UART0_IRQ) {
        // uart_interrupt_handler();
    }
    plic_complete(0, irq);
}

void plicinit() {
    // Set UART0's priority to 1
    plic_set_priority(UART0_IRQ, 1);
    // Enable UART0
    plic_enable(0, UART0_IRQ);
    // Set the threshold to 0
    plic_set_threshold(0, 0);
    *(uint32 *)PLIC_SPRIORITY(0) = 0;
    // Enable external interrupts, done in start.c  
}