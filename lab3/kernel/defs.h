#include "types.h"
#include "page.h"
struct spinlock;
struct proc;

// mm
void  kinit (void);
void *kalloc(void);
void  kfree (void *);
void  memset(void *dst, char c, uint64 n);
void  memcpy(void *dst, void *src, uint64 n);

// physical page
uint64 allocate_page(void);
void   free_page(uint64 pa);

// page-table
void        kptinit(void);
pagetable_t alloc_pagetable(void);
void        map_page(pagetable_t, uint64, uint64, int);
void        unmap_page(pagetable_t, uint64 va);
void        switchpgtbl(pagetable_t);

// proc
void procinit(void);
struct proc *allocproc(void);
void scheduler(void);
void yield(void);
void userinit(void);

// trap
void trapinit(void);
void utrap();
void usertrap();
void utrapret();

// plic
void plicinit(void);

// output
void uartinit();
void echo_input();
void puts(char *s);
char getc(void);
void printf(const char *fmt);
void printfi(const char *fmt, int num);

// sync
void initlock(struct spinlock *lk, char *name);
void acquire (struct spinlock *lk);
void release (struct spinlock *lk);

// user
void test1(void);
void test2(void);
void test3(void);

