#include "defs.h"
#include "page.h"
#include "memlayout.h"
#include "riscv.h"

pagetable_t kernel_pagetable;

void kptinit() {
    kernel_pagetable = (pagetable_t)kalloc();
    memset(kernel_pagetable, 0, PGSIZE);
    if (kernel_pagetable == 0) { printf("kernel_pagetable is 0\n"); }

    // uart
    map_page(kernel_pagetable, UART0, UART0, PTE_R | PTE_W);

    // PLIC
    // map_page(kernel_pagetable, PLIC, PLIC, PTE_R | PTE_W);

    // code
    map_page(kernel_pagetable, KERNBASE, KERNBASE, PTE_R | PTE_X);

    sfence_vma();

    w_satp(MAKESATP(kernel_pagetable));

    // flush stale entries from the TLB.
    sfence_vma();
}

pagetable_t alloc_pagetable() {
    pagetable_t pagetable = (pagetable_t)allocate_page();
    memset(pagetable, 0, PGSIZE);
    return pagetable;
}

void map_page(pagetable_t pagetable, uint64 va, uint64 pa, int perm) {
    uint64 vpn = va / PGSIZE;
    uint64 ppn = pa / PGSIZE;
    pte_t *pte = &pagetable[vpn];
    *pte = (ppn << 10) | perm | PTE_V;
}

void unmap_page(pagetable_t pagetable, uint64 va) {
    uint64 vpn = va / PGSIZE;
    pagetable[vpn] |= ~PTE_V;
}

void switchpgtbl(pagetable_t pagetable) {
    uint64 satp_value = MAKESATP(pagetable);
    asm volatile("csrw satp, %0" :: "r"(satp_value));  // 设置页表基址
    asm volatile("sfence.vma");  // 刷新 TLB
}
