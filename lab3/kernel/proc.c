#include "proc.h"
#include "types.h"
#include "defs.h"
#include "mem.h"
// #include "elf.h"
#include "riscv.h"

struct proc procs[NPROC];
struct proc *current;
struct context scheduler_context;

static uint64 pid = 0;
void userret();


void procinit(void) {
    for (struct proc *p = procs; p < &procs[NPROC]; ++p) {
        p->state = UNUSED;
        p->stack = allocate_page();
        
        p->parent = 0;        
    }
}

struct proc *allocproc(void) {
    for (struct proc *p = procs; p < &procs[NPROC]; ++p) {
        if (p->state == UNUSED) {
            p->state = EMBRYO;
            p->pid = pid++;

            memset(&p->trapframe, 0, sizeof(p->trapframe));
            p->context.sp = p->stack + PGSIZE;
            p->context.ra = userret;
            return p;
        }
    }
    return 0;
}

uint64 sys_fork () {
    struct proc *cp = allocproc();
    struct proc *p  = current;
    if (cp == 0) return -1;

    cp->trapframe = p->trapframe;
    cp->trapframe.sp = allocate_page() + PGSIZE;
    cp->trapframe.epc += 4;

    cp->parent = p;
    cp->trapframe.a0 = 0;
    cp->state = RUNNABLE;

    return cp->pid;
}

uint64 sys_exec () {
    char * name  = (char *)current->trapframe.a0;
    uint64 entry = current->trapframe.a1;


    current->name = name;
    current->trapframe.epc = entry;
    // 重置用户栈指针到栈顶
    current->trapframe.sp =
        // allocate_page() + PGSIZE;
        (current->trapframe.sp + PGSIZE) & ~(PGSIZE - 1);

    userret();

    // if (sys_fork() == 0) {
    //     // why seemingly child process do not jump here?
    //     current->trapframe.epc = entry;
    //     current->trapframe.sp = allocate_page() + PGSIZE;
    //     current->context.ra = userret;
    //     current->state = RUNNABLE;
    // }
    // yield();
}

uint64 sys_exit () {
    current->state = UNUSED;

    // sth went wrong with resource management
    // free_page(current->trapframe.sp);
    // free_page(current->stack);

    // wake up parent in wait
    for (struct proc *p = procs; p < &procs[NPROC]; ++p) {
        if (p != current->parent) continue;
        
        p->state = RUNNABLE;
    }
    swtch(&current->context, &scheduler_context);
    return 0;
}

// xv6's iterate over all procs to find the one to wait
uint64 sys_wait () {
    current->state = SLEEPING;
    // yield();
    swtch(&current->context, &scheduler_context);

    for (struct proc *p = procs; p < &procs[NPROC]; ++p) {
        if (p->state != UNUSED || p->parent != current) continue;
        
        return p->pid;
    }
    return -1;
}

uint64 sys_ps() {
    printf("PID NAME STATE\n");
    for (struct proc *p = procs; p < &procs[NPROC]; ++p) {
        if (p->state == UNUSED) continue;
        printfi("%d   ", p->pid);
        printf(p->name);
        printf(" ");
        switch (p->state) {
            case EMBRYO: printf("EMBRYO"); break;
            case SLEEPING: printf("SLEEPING"); break;
            case RUNNABLE: printf("RUNNABLE"); break;
            case RUNNING: printf("RUNNING"); break;
            case ZOMBIE: printf("ZOMBIE"); break;
        }
        printf("\n");
    }
    return 0;
}

void scheduler() {
    while (1) {
        for (struct proc *p = procs; p < &procs[NPROC]; ++p) {
            if (p->state != RUNNABLE) continue;

            current = p;
            p->state = RUNNING;
            swtch(&scheduler_context, &p->context);
            if (p->state == RUNNING)
            p->state = RUNNABLE;
        }
    }
}

void yield() {
    // printf("Yield\n");
    // w_sstatus(r_sstatus() | 0x2); // 设置 SIE 位
    struct proc *p = current;
    p->state = RUNNABLE;
    swtch(&p->context, &scheduler_context);
}

void user() { 
    char *s = "User\n";
    while (1) {
    asm volatile("mv a1, %0" : : "r"(s));
    asm volatile("li a7, 1");
    asm volatile("ecall");}
    while (1);
}

void init();
void userinit() {
    struct proc *p = allocproc();
    if (p == 0) return;

    // p->context.ra = 0;
    // p->context.sp = p->stack + PGSIZE;

    p->name = "init";
    p->trapframe.epc = (uint64)init;
    p->trapframe.sp = allocate_page() + PGSIZE;
    p->state = RUNNABLE;
}

#define SSTATUS_SPP (1 << 8)
#define SSTATUS_SPIE (1 << 5)
void userret() {
    write_stvec((uint64)utrap);
    unsigned long x = r_sstatus();
    x &= ~SSTATUS_SPP; // clear SPP to 0 for user mode
    x |= SSTATUS_SPIE; // enable interrupts in user mode
    w_sstatus(x);

    current->trapframe.kernel_sp = current->stack + PGSIZE;
    current->trapframe.kernel_trap = (uint64)usertrap;

    // write_stvec((uint64)utrap);
    w_sepc(current->trapframe.epc);
    utrapret();
}

uint64 gettrapframe() {
    return (uint64)&current->trapframe;
}


// void swtch2user() {
//     unsigned long x = r_sstatus();
//     x &= ~SSTATUS_SPP; // clear SPP to 0 for user mode
//     x |= SSTATUS_SPIE; // enable interrupts in user mode
//     w_sstatus(x);

//     w_sepc(current->trapframe.epc);
//     asm volatile("jr %0" : : "r" (current->trapframe.epc));
//     // userret();
//     // asm volatile("sret");
// }

// struct proc *schedule() {
//     for (struct proc *p = procs; p < &procs[NPROC]; ++p) {
//         if (p->state != RUNNABLE) continue;
//         return p;
//     }
//     return 0;
// }

// struct context *swtch(struct context *old) {
//     if (current) {
//         current->context = *old;
//         current->context.sp = (uint64)old;
//     }

//     current = schedule();

//     struct context *new = &current->context;
//     return new;
// }

// Context *on_interrupt (Event ev, Context *ctx) {
//     current->context = *ctx;
//     // Can return any valid Context*
//     current = schedule();
//     return &current->context;
// }

// int
// exec(char *path, char **argv)
// {
//   char *s, *last;
//   int i, off;
//   uint64 argc, sp, stackbase;
//   struct elfhdr elf;
//   struct inode *ip;
//   struct proghdr ph;
//   struct proc *p = myproc();

//   begin_op();

//   if((ip = namei(path)) == 0){
//     end_op();
//     return -1;
//   }
//   ilock(ip);

//   // Check ELF header
//   if(readi(ip, 0, (uint64)&elf, 0, sizeof(elf)) != sizeof(elf))
//     goto bad;

//   if(elf.magic != ELF_MAGIC)
//     goto bad;

//   // Allocate physical memory for the program
//   char *memory = alloc_phys_memory(); // 假设提供物理内存分配函数
//   if(memory == 0)
//     goto bad;

//   uint64 load_address = (uint64)memory;
//   uint64 sz = 0;

//   // Load program into memory
//   for(i = 0, off = elf.phoff; i < elf.phnum; i++, off += sizeof(ph)){
//     if(readi(ip, 0, (uint64)&ph, off, sizeof(ph)) != sizeof(ph))
//       goto bad;
//     if(ph.type != ELF_PROG_LOAD)
//       continue;
//     if(ph.memsz < ph.filesz)
//       goto bad;
//     if(ph.vaddr + ph.memsz < ph.vaddr)
//       goto bad;

//     // Directly load segments into physical memory
//     if(loadseg(load_address + ph.vaddr, ip, ph.off, ph.filesz) < 0)
//       goto bad;
//     sz = ph.vaddr + ph.memsz;
//   }
//   iunlockput(ip);
//   end_op();
//   ip = 0;

//   p = myproc();

//   // Allocate stack
//   stackbase = load_address + sz;       // 栈紧挨着程序
//   sp = stackbase + PGSIZE;             // 假设栈大小为 PGSIZE

//   // Push argument strings, prepare rest of stack
//   uint64 ustack[MAXARG];
//   for(argc = 0; argv[argc]; argc++) {
//     if(argc >= MAXARG)
//       goto bad;
//     sp -= strlen(argv[argc]) + 1;
//     sp -= sp % 16; // sp must be 16-byte aligned
//     if(sp < stackbase)
//       goto bad;
//     memcpy((void *)sp, argv[argc], strlen(argv[argc]) + 1);
//     ustack[argc] = sp;
//   }
//   ustack[argc] = 0;

//   // Push the array of argv[] pointers
//   sp -= (argc + 1) * sizeof(uint64);
//   sp -= sp % 16;
//   if(sp < stackbase)
//     goto bad;
//   memcpy((void *)sp, ustack, (argc + 1) * sizeof(uint64));

//   // Setup trapframe
//   p->trapframe->a1 = sp;            // argv
//   p->trapframe->epc = elf.entry;    // initial program counter = main
//   p->trapframe->sp = sp;            // initial stack pointer

//   // Save program name for debugging
//   for(last = s = path; *s; s++)
//     if(*s == '/')
//       last = s + 1;
//   safestrcpy(p->name, last, sizeof(p->name));

//   return argc;

//  bad:
//   if(ip){
//     iunlockput(ip);
//     end_op();
//   }
//   return -1;
// }