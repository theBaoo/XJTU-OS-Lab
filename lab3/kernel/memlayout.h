// memlayout.h - 内存布局定义
#define UART0 0x10000000L                   // UART0 基地址, used for put char.
#define KERNBASE 0x80000000L
#define PHYSTOP (KERNBASE + 128*1024*1024)
#define PLIC 0x0C000000L

#define PTE2PA(pte) (((pte) >> 10) << 12)
#define PA2PTE(pa)  (((pa)  >> 12) << 10)