#include "defs.h"
#include "types.h"
#include "riscv.h"
#include "trap.h"
#include "syscall.h"
#include "proc.h"

void timer_interrupt();
void strap();

int timepoint = 0;

void check_machine_mode() {
    uint64 mstatus = r_mstatus();
    uint64 mpp = (mstatus & MSTATUS_MPP_MASK) >> 11;
    if (mpp != 3) {
        printf("Not in machine mode!\n");
        while (1); // 停止执行
    }
}

void __attribute__((aligned(4))) trap_handler() {
    uint64 scause = r_scause();
    uint64 sepc   = r_sepc();
    uint64 sstatus = r_sstatus();
    if (((scause) >> 63)) {
        uint64 interrupt = scause & 0xfff;
        if (interrupt == 1) {
            // ++timepoint;
            w_sip(r_sip() & ~2);
            yield();

        // w_sepc(sepc + 4);
        // asm volatile("ret");
        } else if (interrupt == 9) {
            int irq = plic_claim(0);
            if (irq == 9) {
                uartintr();
            } 
            plic_complete(0, irq);
            // w_sepc(sepc + 4);    
        } else {
            printfi("unexpected interrupt %d\n", interrupt);
        }
    } else {
        if (scause == 9 || scause == 8) {
            syscall();
            w_sepc(sepc + 4);
        } else {
            // printfi("unexpected exception %d\n", scause);
            w_sepc(sepc + 4);
        }
    }
    // w_sstatus(sstatus);
    return;
}

void __attribute__((aligned(4))) mtrap() {
    ++timepoint;
    asm volatile("csrrw a0, mscratch, a0");
    asm volatile("sd a1, 0(a0)");
    asm volatile("sd a2, 8(a0)");
    asm volatile("sd a3, 16(a0)");

    asm volatile("ld a1, 24(a0) ");
    asm volatile("ld a2, 32(a0)");
    asm volatile("ld a3, 0(a1)");
    asm volatile("add a3, a3, a2");
    asm volatile("sd a3, 0(a1)");

    w_sip(r_sip() | 2);
    // asm volatile("li a1, 2");
    // asm volatile("csrw sip, a1");

    asm volatile("ld a1, 0(a0)");
    asm volatile("ld a2, 8(a0)");
    asm volatile("ld a3, 16(a0)");
    asm volatile("csrrw a0, mscratch, a0");

    // w_mepc(r_mepc() + 4);
    asm volatile("mret");
}

extern void userret();
// utrap jumps here.
void __attribute__((aligned(4))) usertrap() {
    write_stvec((uint64)trap_handler);
    current->trapframe.epc = r_sepc();

    if (r_scause() == UECALL) {
        syscall();
        current->trapframe.epc += 4;
    } else if (r_scause() == 0x8000000000000001) {
        w_sip(r_sip() & ~2);
        // ++timepoint;
        yield();
    } else if (r_scause() == 0x8000000000000009) {
        int irq = plic_claim(0);
        if (irq == 9) uartintr();
        if (irq) plic_complete(0, irq);
    } else {
        // current->trapframe.epc += 4;
 
        printfi("unexpected scause %d\n", r_scause());
    }

    userret();
}

void trapinit() {
    // check_machine_mode();
    uint64 strap_addr = (uint64)strap;
    strap_addr &= ~0x3;
    write_stvec(strap_addr);
}