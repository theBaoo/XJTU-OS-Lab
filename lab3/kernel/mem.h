#ifndef MEM_H
#define MEM_H
#define PGSIZE  4096
// #define PHYSTOP 0x88000000

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))

struct run {
    struct run *next;
};

struct kmem {
    struct run *freelist;
};

extern struct kmem kmem;
extern char end[]; // first address after kernel loaded from ELF file

#endif