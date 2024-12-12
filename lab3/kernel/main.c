#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"
#include "mem.h"
#include "memlayout.h"
#include "spinlock.h"
#include "syscall.h"

void
main()
{
    printf("\nKernel is booting.\n");

    kinit();
    printf("kinit done.\n");

    // kptinit();
    // printf("kptinit done.\n");

    procinit();
    printf("procinit done.\n");

    uartinit();
    printf("uartinit done.\n");

    // plicinit();
    
    userinit();
    printf("userinit done.\n");

    scheduler();

    printf("Ooops! Kernel is stuck.\n"); 
    while (1) {
        asm volatile ("wfi");
    }
}