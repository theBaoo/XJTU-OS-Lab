#include "types.h"

#define UECALL 8

#define SYS_write 1
#define SYS_read  2
#define SYS_fork  3
#define SYS_exec  4
#define SYS_exit  5
#define SYS_wait  6
#define SYS_sleep 7
#define SYS_ps    8


struct syscall_item {
    int syscall_num;
    uint64 (*syscall_handler)();
};

extern struct syscall_item syscalls[];

uint64 sys_write();
uint64 sys_read ();
uint64 sys_fork ();
uint64 sys_exec ();
uint64 sys_exit ();
uint64 sys_wait ();
uint64 sys_sleep();
uint64 sys_ps   ();

void syscall();