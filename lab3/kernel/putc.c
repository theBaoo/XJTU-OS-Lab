#include "types.h"
#include "riscv.h"
#include "memlayout.h"
#include "defs.h"

#define IER 1
#define FCR 2
#define LCR 3
#define LCR_BAUD_LATCH (1 << 7)
#define LCR_EIGHT_BITS (3 << 0)
#define FCR_FIFO_ENABLE (1 << 0)
#define FCR_FIFO_CLEAR (3 << 1)
#define IER_TX_ENABLE (1 << 1)
#define IER_RX_ENABLE (1 << 0)

#define Reg(reg) ((volatile uchar *)(UART0 + reg))
#define WriteReg(reg, v) (*(Reg(reg)) = (v))
#define ReadReg(reg) (*(Reg(reg)))
#define WaitFIFO() while ((*(Reg(5)) & 0x20) == 0)
#define HasData (*(Reg(5)) & 1)

#define BFSIZE 128

static char buf[BFSIZE];
static int rdx = 0;
static int wdx = 0;

void
uartinit()
{
    *Reg(IER) = 0;
    *Reg(LCR) = LCR_BAUD_LATCH;
    *Reg(0) = 0x3; // 2400 baud
    *Reg(1) = 0;
    *Reg(3) = LCR_EIGHT_BITS;
    *Reg(FCR) = FCR_FIFO_CLEAR | FCR_FIFO_ENABLE;
    *Reg(IER) = IER_RX_ENABLE | IER_TX_ENABLE;
}

void 
uartintr() 
{
    while (*Reg(5) & 1) {
        char c = ReadReg(0);
        if ((wdx + 1) % BFSIZE != rdx) {
            buf[wdx] = c;
            wdx = (wdx + 1) % BFSIZE;
        } else {
            
        }
    }
}

void echo_input() {
    char c;
    while (1) {
        c = getc(); // 获取字符
        putc(c);    // 回显字符
        if (c == '\r') {    // 处理换行符
            putc('\n');
        }
    }
}


// void
// uartintr()
// {
//     volatile uchar *uart = (uchar *)UART0;
//     char c;

//     if (uart[5] & 1) {
//         c = getc();         
//         putc(c);     
//         if (c == '\r') {
//             putc('\n');
//         }
//     }
// }

void
putc(char c)
{
    WaitFIFO();
    WriteReg(0, c);
}

char
getc()
{
    // while (HasData);
    // return ReadReg(0);
    if (HasData)
        return ReadReg(0);
    else
        return 0;
    
    // while (rdx == wdx) {
    //     asm volatile("wfi");
    // }
    // char c = buf[rdx];
    // rdx = (rdx + 1) % BFSIZE;
    // return c;
}

// correct.
void uarttest() {
    putc('H');
    putc('e');
    putc('l');

    while (HasData) {
        putc(getc());
        putc(getc());
        putc(getc());
    }
}