struct spinlock {
    int locked;
    char *name;
};

static inline int 
_atomic_xchg(volatile int *addr, int newval);