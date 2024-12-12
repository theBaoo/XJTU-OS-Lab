// printf.c - 简单的 printf 实现
#include "types.h"
#include "defs.h"
#include "riscv.h"

static char digits[] = "0123456789abcdef";

void
printf(const char *fmt) {
    while (*fmt) {
        putc(*fmt);
        fmt++;
    }
}

void
printint(int n)
{
    if (n == 0) { putc('0'); return; }
    if (n < 0 ) { putc('-'); n = -n; }
    char buf[16];
    volatile int i = 0;
    while (n) {
        buf[i++] = digits[n % 10];
        n /= 10;
    }
    while (i > 0) {
        putc(buf[--i]);
    }
}

void
printfi(const char *fmt, int n)
{
    int i, c;
 
    // uint64 sie = r_sie();
    // w_sie(sie & ~(1 << 1)); // 清除 STIE 位，禁用定时器中断

    for(i = 0; fmt[i]; i++) {
        if(fmt[i] != '%') {
            putc(fmt[i]);
            continue;
        }
        switch(fmt[++i]) {
        case 'd':
            printint(n);
            break;
        default:
            putc('%');
            putc(fmt[i]);
            break;
        }
    }

    // w_sie(sie); // 恢复 STIE 位
}