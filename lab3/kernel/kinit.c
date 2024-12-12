#include "defs.h"
#include "types.h"
#include "mem.h"
#include "memlayout.h"

struct kmem kmem;

void kinit() {
    char *p = (char *)PGROUNDUP((uint64)end);
    for (; p + PGSIZE <= (char *)PHYSTOP; p += PGSIZE) {
        struct run *r = (struct run *)p;
        r->next = kmem.freelist;
        kmem.freelist = r;
    }
}

void *kalloc() {
    if (kmem.freelist == 0) return 0;

    struct run *r;
    r = kmem.freelist;
    kmem.freelist = r->next;
    return (void *)r;
}

void kfree(void *p) {
    if ((uint64)p % PGSIZE != 0 || (char *)p < end || (uint64)p >= PHYSTOP) return;

    char *pa = (char *)p;
    for (int i = 0; i < PGSIZE; ++i) pa[i] = 0;
    
    struct run *r;
    r = (struct run *)p;
    r->next = kmem.freelist;
    kmem.freelist = r;
}

uint64 allocate_page() {
    void *p = kalloc();
    if (p == 0) return 0;
    return (uint64)p;
}

void free_page(uint64 pa) {
    kfree((void *)pa);
}

void memset(void *dst, char c, uint64 n) {
    for (int i = 0; i < n; ++i) ((char *)dst)[i] = c;
}

void memcpy(void *dst, void *src, uint64 n) {
    for (int i = 0; i < n; ++i) ((char *)dst)[i] = ((char *)src)[i];
}
