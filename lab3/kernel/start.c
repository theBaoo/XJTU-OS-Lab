// start.c - 内核启动代码
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

void main();
void timerinit();
void mtrap();
void strap();

// entry.S 需要每个 CPU 一个栈。
__attribute__ ((aligned (16))) char stack0[4096 * NCPU];

// 每个 CPU 的机器模式定时器中断的临时区域。
uint64 timer_scratch[NCPU][5];

// entry.S 在机器模式下跳转到这里。
void
start()
{
    unsigned long x = r_mstatus();
    x &= ~MSTATUS_MPP_MASK;
    x |= MSTATUS_MPP_S;
    w_mstatus(x);

    w_mepc((uint64)main);

    // 暂时禁用分页。
    w_satp(0);

    // 将所有中断和异常委托给 Supervisor 模式。
    w_medeleg(0xffff);
    w_mideleg(0xffff);
    w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);

    // 配置物理内存保护，以便 Supervisor 模式访问所有物理内存。
    w_pmpaddr0(0x3fffffffffffffull);
    w_pmpcfg0(0xf);

    // 请求时钟中断。
    timerinit();
    trapinit();

    // 将每个 CPU 的 hartid 保存在 tp 寄存器中，以便 cpuid()。
    int id = r_mhartid();
    w_tp(id);

       w_mstatus(r_mstatus() | MSTATUS_MIE);
    // enable global interrupt
    w_sstatus(r_sstatus() | 0x2); // 设置 SIE 位

    // 切换到 Supervisor 模式并跳转到 main()。
    asm volatile("mret");
}