#include "types.h"
#include "riscv.h"
#include "param.h"
#include "proc.h"

extern int timepoint;

void mtrap();
extern uint64 timer_scratch[NCPU][5];
void timerinit() { 
    int interval_init = 5000000;
    *(volatile uint64*)MTIMECMP =
        *(volatile uint64 *)MTIME + interval_init;
    
    int interval = 1000000;
    timer_scratch[0][3] = MTIMECMP;
    timer_scratch[0][4] = interval;
    w_mscratch((uint64)timer_scratch);

    w_mtvec((uint64)mtrap);
    w_mstatus(r_mstatus() | 0x8);
    w_mie(r_mie() | MIE_MTIE);
}

uint64 sys_sleep() {
    printf("Not implemented yet\n");
    // int time = current->trapframe.a0;
    // int cur = timepoint;
    // while (timepoint - cur < time);
    // return 0;
}
