#include "syscall.h"
#include "proc.h"

struct syscall_item syscalls[] = {
    {SYS_write, sys_write},
    {SYS_read,  sys_read },
    {SYS_fork,  sys_fork },
    {SYS_exec,  sys_exec },
    {SYS_exit,  sys_exit },
    {SYS_wait,  sys_wait },
    {SYS_sleep, sys_sleep},
    {SYS_ps,    sys_ps   },
};
int syscalls_num = sizeof(syscalls) / sizeof(syscalls[0]);

void syscall() {
    int syscall_num;
    syscall_num = current->trapframe.a7;

    if (syscall_num > 0 && syscall_num <= syscalls_num 
        && syscalls[syscall_num - 1].syscall_handler != 0) {
        current->trapframe.a0 =
            syscalls[syscall_num - 1].syscall_handler();
    }
}

uint64 sys_write() {
    int fd;
    char *buf;
    int len;
    int hasnum = 0;
    buf = (char *)current->trapframe.a1;
    hasnum = current->trapframe.a2;
    // printf(buf);
    if (!hasnum) printf(buf);
    else printfi(buf, hasnum);
    return len;
}

uint64 sys_read () {
    int fd;
    char *buf;
    int len;
    int num = 0;
    buf = (char *)current->trapframe.a1;
    len = current->trapframe.a2;
    if (len == 0) len = 128;
    // while ((buf[num++] = getc()) != '\n') {
    //     putc(buf[num - 1]);
    // }
    // for (int i = 0; i < current->trapframe.a2; i++) {
    //     while ((buf[i] = getc()) == 0);
    //     putc(buf[i]);
    //     num++;
    //     if (buf[i] == '\n') break;
    // }
    // buf[num - 1] = '\0';
    // putc('\n');
    for (int i = 0; i < len; i++) {
        while ((buf[i] = getc()) == 0);
        if (buf[i] == '\r') {
            putc('\n');
            break;
        } else if (buf[i] == 127) {
            putc('\b'); putc(' '); putc('\b');
            if (num > 0) num--;
            if (i > 0) i--;
        } else {
            putc(buf[i]);
            num++;
        }
    }
    buf[num] = '\0';
    return num;
}
