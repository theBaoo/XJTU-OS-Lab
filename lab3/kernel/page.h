#include "types.h"

#define PGSIZE  4096
#define PTSIZE  512
#define PTE_V   0x001  // Valid
#define PTE_R   0x002  // Read
#define PTE_W   0x004  // Write
#define PTE_X   0x008  // Execute
#define PTE_U   0x010  // User

#define MAKESATP(pagetable) ((8L << 60) | (uint64)(pagetable) >> 12)

typedef uint64 pte_t;   // 页表条目
typedef pte_t* pagetable_t;  // 页表指针

extern pagetable_t kernel_pagetable;