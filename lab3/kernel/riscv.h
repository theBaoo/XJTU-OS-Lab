// riscv.h - RISC-V 特定寄存器操作
#define MSTATUS_MPP_MASK (3L << 11)
#define MSTATUS_MPP_S (1L << 11)
#define SIE_SEIE (1L << 9)  // 外部中断使能
#define SIE_STIE (1L << 5)  // 定时器中断使能
#define SIE_SSIE (1L << 1)  // 软件中断使能

#define MTIME          ((volatile uint64 *)(CLINT_BASE + 0xBFF8))
#define MTIMECMP       ((volatile uint64 *)(CLINT_BASE + 0x4000))
#define CLINT_BASE     0x02000000L

#define MIE_MTIE       (1L << 7)   // Machine Timer Interrupt Enable
#define MSTATUS_MIE    (1L << 3)   // Machine Interrupt Enable
static inline void
w_mtvec(uint64 x)
{
    asm volatile("csrw mtvec, %0" : : "r" (x));
}

static inline void
write_stvec(uint64 x)
{
    asm volatile("csrw stvec, %0" : : "r" (x));
}

static inline uint64
r_mcause()
{
    uint64 x;
    asm volatile("csrr %0, mcause" : "=r" (x) );
    return x;
}

static inline uint64
r_scause()
{
    uint64 x;
    asm volatile("csrr %0, scause" : "=r" (x) );
    return x;
}

static inline uint64
r_sepc()
{
    uint64 x;
    asm volatile("csrr %0, sepc" : "=r" (x) );
    return x;
}

static inline void
w_sepc(uint64 x)
{
    asm volatile("csrw sepc, %0" : : "r" (x));
}

static inline uint64
r_time()
{
    uint64 x;
    asm volatile("csrr %0, time" : "=r" (x));
    return x;
}

static inline void
w_stimecmp(uint64 x)
{
    asm volatile("csrw stimecmp, %0" : : "r" (x));
}

static inline uint64
r_sip()
{
    uint64 x;
    asm volatile("csrr %0, sip" : "=r" (x));
    return x;
}

static inline void
w_sip(uint64 x)
{
    asm volatile("csrw sip, %0" : : "r" (x));
}

static inline uint64
r_sstatus()
{
    uint64 x;
    asm volatile("csrr %0, sstatus" : "=r" (x) );
    return x;
}

static inline void
w_sstatus(uint64 x)
{
    asm volatile("csrw sstatus, %0" : : "r" (x));
}

static inline void
w_mie(uint64 x)
{
    asm volatile("csrw mie, %0" : : "r" (x));
}

static inline uint64
r_mie()
{
    uint64 x;
    asm volatile("csrr %0, mie" : "=r" (x) );
    return x;
}

static inline void
w_mip(uint64 x)
{
    asm volatile("csrw mip, %0" : : "r" (x));
}

static inline uint64
r_mip()
{
    uint64 x;
    asm volatile("csrr %0, mip" : "=r" (x) );
    return x;
}

static inline uint64
r_mstatus()
{
    uint64 x;
    asm volatile("csrr %0, mstatus" : "=r" (x) );
    return x;
}

static inline void
w_mstatus(uint64 x)
{
    asm volatile("csrw mstatus, %0" : : "r" (x));
}

static inline void
w_mepc(uint64 x)
{
    asm volatile("csrw mepc, %0" : : "r" (x));
}

static inline uint64
r_mepc()
{
    uint64 x;
    asm volatile("csrr %0, mepc" : "=r" (x) );
    return x;
}

static inline void
w_satp(uint64 x)
{
    asm volatile("csrw satp, %0" : : "r" (x));
}

static inline void
w_medeleg(uint64 x)
{
    asm volatile("csrw medeleg, %0" : : "r" (x));
}

static inline void
w_mideleg(uint64 x)
{
    asm volatile("csrw mideleg, %0" : : "r" (x));
}

static inline uint64
r_mideleg()
{
    uint64 x;
    asm volatile("csrr %0, mideleg" : "=r" (x) );
    return x;
}

static inline uint64
r_sie()
{
    uint64 x;
    asm volatile("csrr %0, sie" : "=r" (x) );
    return x;
}

static inline void
w_sie(uint64 x)
{
    asm volatile("csrw sie, %0" : : "r" (x));
}

static inline void
w_pmpaddr0(uint64 x)
{
    asm volatile("csrw pmpaddr0, %0" : : "r" (x));
}

static inline void
w_pmpcfg0(uint64 x)
{
    asm volatile("csrw pmpcfg0, %0" : : "r" (x));
}

static inline uint64
r_mhartid()
{
    uint64 x;
    asm volatile("csrr %0, mhartid" : "=r" (x) );
    return x;
}

static inline void
w_tp(uint64 x)
{
    asm volatile("mv tp, %0" : : "r" (x));
}

static inline void
w_mscratch(uint64 x)
{
    asm volatile("csrw mscratch, %0" : : "r" (x));
}

// flush the TLB.
static inline void
sfence_vma()
{
  // the zero, zero means flush all TLB entries.
  asm volatile("sfence.vma zero, zero");
}