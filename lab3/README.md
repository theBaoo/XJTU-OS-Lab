# todo
- one-level pagetable          ❌
    - modify map_page declaration by adding size ?
    - modify proc stack alloc
    - modify scheduler satp-switch
- init user program            ✅
    - include the machine code ❌
    - sret to umode to execute ❌
    - and test the syscall     ✅
- keyboard interrupt
    - able to ineract with.    
    - uartintr
- syscall in u mode             ✅
    - utrap for store trapframe ✅
    - 
- improve shell
    - make init start sh.       ✅
# usages
- `make qemu`: load and execute the kernel
- `make qemu-gdb` and in another terminal `gdb-multiarch -x init.py`: debug the kernel

# dev
## stage I: a simple kernel
- boot in qemu               11.6
- physical memory and page   11.17
- scheduler                  11.17
- syscall and trap in S mode 11.19
- timer interrupt            11.19 ?

## stage II: the kernel improved from 3 aspects
### concurrency
- lock 
### virtualization
###

# bugs

## link
riscv64-unknown-elf-ld: kernel/entry.o: can't link double-float modules with soft-float modules

## physics addr is set wrongly(fixed)

## ???
![alt text](image.png)

## timer interrupt in M mode cannot jump to trap handler in S mode

## sys_write failed in several times, scause is 0x7

## sie in sstatus is changed when timer interrupt, causing only one yield happening.
Maybe hardware change that.
Recrrect sstatus in yield() works.
Found that sstatus change when mtrap jump to strap.
Compared to xv6, maybe sstatus & 0x2 has nothing to do with timer interrupt???

## spinlock is interrupted
when returning to test1, scause = 4 or 5(sepc: acquire(&lk)) ???\
*solved*. Before acquire, int must be disabled!!\
why a thread do not spin when the lock is locked until next timer interrupt??????????

## deadlock
Interrupt disabled before acquire && release lock, deadlock could happened sometimes.
And why sometimes sum would reaccumulate from 0 ???

## a1 in trapframe may change when syscall.
using gdb's watch to debug now.
![alt text](image-1.png)
Maybe sth went wrong with user programe.
Or address space went wrong?

## w_stvec failed without exception
for the address is not aligned with 4

## plic: s or no-s? Maybe sth went wrong with uartinit
test in s mode.

## The return value of gets is 'optimized' ?
Cause failing get the num of input.

## The param a1 is wrong ???
## also a2 in gets, causing read nothing

## in utrap, ra is changed, for the addr of trapframe is returned by a function. But why it worked well before???

## sth went wrong with ulib::strlen.

## child process after fork has wrong ra.

## sth went wrong with kfree.

## ufork has bug. child process do not return 0 but 2.
## uwait

## exit failed in modifying the state of process
scheduler和yield有问题, 没有考虑情况进程的状态

为proc添加name字段, 方便调试

## wth wrong with uwait in init.
If the sepc is recorrected, reboot sh works well.
疑似是运算顺序`pid = ufork() == 0`