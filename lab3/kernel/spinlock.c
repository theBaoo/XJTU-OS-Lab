#include "spinlock.h"

void initlock(struct spinlock *lk, char *name) {
    lk->name = name;
    lk->locked = 0;
}

void acquire (struct spinlock *lk) {
    while (__sync_lock_test_and_set(&lk->locked, 1) != 0);
}
void release (struct spinlock *lk) {
    __sync_lock_release(&lk->locked);
}

static inline int 
_atomic_xchg(volatile int *addr, int newval)
{
    int result;
    __asm__ __volatile__ (
        "amoswap.w %0, %2, %1"
        : "=r" (result), "+A" (*addr)
        : "r" (newval)
        : "memory"
    );
    return result;
}