#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define PROCESS_NAME_LEN  32   /*进程名长度   */
#define MIN_SLICE         10   /*最小碎片的大小*/
#define DEFAULT_MEM_SIZE  1024 /*内存大小     */
#define DEFAULT_MEM_START 0    /*起始位置     */

/* 内存分配算法 */
#define MA_FF             1
#define MA_BF             2
#define MA_WF             3
int mem_size     = DEFAULT_MEM_SIZE; /*内存大小*/
int ma_algorithm = MA_FF;            /*当前分配算法*/
static 
int pid        = 0;             /*初始 pid*/
int flag       = 0;             /*设置内存大小标志*/
int menu_items = 6;             /*菜单选项个数*/

struct free_block_type {
    int size;
    int start_addr;
    struct free_block_type *next;
} *free_block;

struct allocated_block {
    int pid;
    int size;
    int start_addr;
    char process_name[PROCESS_NAME_LEN];
    struct allocated_block *next;
} *allocated_block_head;

char *menu[] = {
    "1 - Set memory size (default = " TOSTRING(DEFAULT_MEM_SIZE) ")\n",
    "2 - Select memory allocation algorithm                        \n",
    "3 - New process                                               \n",
    "4 - Terminate a process                                       \n",
    "5 - Display memory usage                                      \n",
    "0 - Exit                                                      \n"
};

void display_menu();
struct free_block_type* init_free_block(int mem_size);
int  set_mem_size();
void set_algorithm();
void rearrange();
void rearrange_FF();
void rearrange_BF();
void rearrange_WF();
void sorted_insert(struct free_block_type **head_ref, struct free_block_type *new_node);
void insertion_sort(struct free_block_type **head_ref);
void memory_compaction();
int  new_process();
int  allocate_mem(struct allocated_block *ab);
void kill_process();
int  free_mem(struct allocated_block *ab);
int  dispose(struct allocated_block *free_ab);
int  display_mem_usage();
int  do_exit();
struct allocated_block *find_process(int pid);
void wait_key();


int main() {
    char choice; pid=0;

#ifndef TEST
    free_block = init_free_block(mem_size); //初始化空闲区
#else
    struct free_block_type *free_block1 = (struct free_block_type *)malloc(sizeof(struct free_block_type));
    struct free_block_type *free_block2 = (struct free_block_type *)malloc(sizeof(struct free_block_type));
    struct free_block_type *free_block3 = (struct free_block_type *)malloc(sizeof(struct free_block_type));

    free_block1->size = 100;
    free_block1->start_addr = 0;
    free_block1->next = free_block2;

    free_block2->size = 200;
    free_block2->start_addr = 200;
    free_block2->next = free_block3;

    free_block3->size = 300;
    free_block3->start_addr = 500;
    free_block3->next = NULL;

    free_block = free_block1;
    mem_size = 600;
#endif

    while(1) {

#ifndef TEST
        display_menu(); //显示菜单
#endif
        // fflush(stdin);
        choice = getchar(); //获取用户输入
        switch(choice) {
            case '1': set_mem_size();              wait_key(); break; //设置内存大小
            case '2': set_algorithm();     flag=1; wait_key(); break; //设置算法
            case '3': new_process();       flag=1; wait_key(); break; //创建新进程
            case '4': kill_process();      flag=1; wait_key(); break; //删除进程
            case '5': display_mem_usage(); flag=1; wait_key(); break; //显示内存使用
            case '0': do_exit();           exit(0);       //释放链表并退出
            default: break; 
        }
    }
}

void wait_key() {
#ifndef TEST
    printf("Press any key to continue...");
    while (getchar() != '\n');
    (void)getchar();
#endif
}

struct free_block_type* init_free_block(int mem_size) {
    struct free_block_type *fb 
        = (struct free_block_type *)malloc(sizeof(struct free_block_type));

    if (fb == NULL) {
        printf("No mem.\n");
        return NULL;
    }

    fb->size       = mem_size;
    fb->start_addr = DEFAULT_MEM_START;
    fb->next       = NULL;

    return fb;
}

void display_menu() {
    printf("\n");
    printf("======================================\n");
    for (int i = 0; i < menu_items; i++) {
        printf("%s", menu[i]);
    }
    printf("======================================\n");
}

int set_mem_size() {
    int size;
    if ( flag !=0 ) { //防止重复设置
        printf("Cannot set memory size again.\n");
        return 0;
    }
#ifndef TEST
    printf("Total memory size =");
#endif
    scanf("%d", &size);
    if (size > 0) {
        mem_size = size;
        free_block->size = mem_size;
    }
    flag = 1;
    return 1;
}

void set_algorithm() {
    int algorithm;
#ifndef TEST
    printf("\t1 - First Fit\n");
    printf("\t2 - Best  Fit\n");
    printf("\t3 - Worst Fit\n");
#endif
    scanf("%d", &algorithm);
    if(algorithm>=1 && algorithm <=3) ma_algorithm = algorithm;
    //按指定算法重新排列空闲区链表
    rearrange();
}

void rearrange() {
    switch(ma_algorithm){
        case MA_FF: rearrange_FF(); break;
        case MA_BF: rearrange_BF(); break;
        case MA_WF: rearrange_WF(); break;
    }
}

void rearrange_FF() {
    struct free_block_type *p, *q;
    int temp;
    for (p = free_block; p != NULL; p = p->next) {
        for (q = p->next; q != NULL; q = q->next) {
            if (p->start_addr > q->start_addr) {
                // 交换 start_addr
                temp = p->start_addr;
                p->start_addr = q->start_addr;
                q->start_addr = temp;
                // 交换 size
                temp = p->size;
                p->size = q->size;
                q->size = temp;
            }
        }
    }
}

void rearrange_BF() {
    struct free_block_type *p, *q;
    int temp;
    for (p = free_block; p != NULL; p = p->next) {
        for (q = p->next; q != NULL; q = q->next) {
            if (p->size > q->size) {
                // 交换 start_addr
                temp = p->start_addr;
                p->start_addr = q->start_addr;
                q->start_addr = temp;
                // 交换 size
                temp = p->size;
                p->size = q->size;
                q->size = temp;
            }
        }
    }
}

void rearrange_WF() {
    struct free_block_type *p, *q;
    int temp;
    for (p = free_block; p != NULL; p = p->next) {
        for (q = p->next; q != NULL; q = q->next) {
            if (p->size < q->size) {
                // 交换 start_addr
                temp = p->start_addr;
                p->start_addr = q->start_addr;
                q->start_addr = temp;
                // 交换 size
                temp = p->size;
                p->size = q->size;
                q->size = temp;
            }
        }
    }
}

int new_process() {
    struct allocated_block *ab;
    int size;
    int ret ;
    ab = (struct allocated_block *)malloc(sizeof(struct allocated_block));
    if (!ab) exit(-5);
    ab->next = NULL;
    pid++;
    sprintf(ab->process_name, "PROCESS-%02d", pid);
    ab->pid = pid;
#ifndef TEST
    printf("Memory for %s:", ab->process_name);
#endif
    scanf("%d", &size);
    if(size > 0) ab->size = size;
    ret = allocate_mem(ab); /* 从空闲区分配内存， ret==1 表示分配 ok*/
    /*如果此时 allocated_block_head 尚未赋值，则赋值*/
    if ( ret == -1) {
        printf("Allocation failed.\n");
        free(ab);
        return -1; 
    }

    if ((ret == 1) && (allocated_block_head == NULL)) {
        allocated_block_head = ab;
        return 1;
    } else if (ret == 1) { /*分配成功，将该已分配块的描述插入已分配链表*/
        ab->next = allocated_block_head;
        allocated_block_head = ab;
        return 2;
    }
    return 3;
}

int allocate_mem(struct allocated_block *ab) {
    struct free_block_type *fbt, *pre;
    int request_size = ab->size;
    fbt = pre = free_block;
    while (fbt != NULL) {
        if (fbt->size >= request_size) {
            ab->start_addr = fbt->start_addr;
            fbt->size -= request_size;
            fbt->start_addr += request_size;

            if (fbt->size > MIN_SLICE) return 1;

            ab->size += fbt->size;
            if (fbt == free_block) {
                free_block = fbt->next;
            } else {
                pre->next = fbt->next;
            }
            free(fbt);

            return 1;
        }

        pre = fbt;
        fbt = fbt->next;
    }

    memory_compaction();
    fbt = pre = free_block;
    while (fbt != NULL) {
        if (fbt->size >= request_size) {
            ab->start_addr = fbt->start_addr;
            fbt->size -= request_size;
            fbt->start_addr += request_size;

            if (fbt->size > MIN_SLICE) return 1;

            ab->size += fbt->size;
            if (fbt == free_block) {
                free_block = fbt->next;
            } else {
                pre->next = fbt->next;
            }
            free(fbt);
            rearrange();
            return 1;
        }

        pre = fbt;
        fbt = fbt->next;
    }
    return -1;
    //根据当前算法在空闲分区链表中搜索合适空闲分区进行分配，分配时注意以下情况：
    // 1. 找到可满足空闲分区且分配后剩余空间足够大，则分割
    // 2. 找到可满足空闲分区且但分配后剩余空间比较小，则一起分配
    // 3. 找不可满足需要的空闲分区但空闲分区之和能满足需要，则采用内存紧缩技术, 进行空闲分区的合并，然后再分配
    // 4. 在成功分配内存后，应保持空闲分区按照相应算法有序
    // 5. 分配成功则返回 1，否则返回-1
    //请自行补充。。。。。
}

void memory_compaction() {
    struct allocated_block *ab = allocated_block_head;

    int new_start_addr = 0;
    while (ab) {
        if (ab->start_addr != new_start_addr) {
            ab->start_addr = new_start_addr;
        }
        new_start_addr += ab->size;
        ab = ab->next;
    }

    struct free_block_type *fbt = (struct free_block_type*) malloc(sizeof(struct free_block_type));
    struct free_block_type *work = free_block;
    fbt->start_addr = new_start_addr;
    fbt->size = mem_size - new_start_addr;
    fbt->next = NULL;
    while (work) {
        free_block = work->next;
        free(work);
        work = free_block;
    }
    free_block = fbt;
}

void kill_process() {
    struct allocated_block *ab;
    int pid;
#ifndef TEST
    printf("Kill Process, pid = ");
#endif
    scanf("%d", &pid);
    ab = find_process(pid);
    if (ab != NULL) {
        free_mem(ab); /*释放 ab 所表示的分配区*/
        dispose (ab); /*释放 ab 数据结构节点*/
    }
}

int free_mem(struct allocated_block *ab) {
    struct free_block_type *fbt, *work;
    fbt = (struct free_block_type*) malloc(sizeof(struct free_block_type));
    if(!fbt) return -1;

    fbt->size = ab->size;
    fbt->start_addr = ab->start_addr;
    fbt->next = NULL;
    
    if (free_block == NULL) { free_block = fbt; return 1; }
    work = free_block;
    while (work->next) work = work->next;
    work->next = fbt;

    insertion_sort(&free_block);
    work = free_block;
    while (work->next) {
        if (work->start_addr + work->size == work->next->start_addr) {
            work->size += work->next->size;
            work->next = work->next->next;
        } else {
            work = work->next;
        }
    }

    rearrange();
    // 进行可能的合并，基本策略如下
    // 1. 将新释放的结点插入到空闲分区队列末尾
    // 2. 对空闲链表按照地址有序排列
    // 3. 检查并合并相邻的空闲分区
    // 4. 将空闲链表重新按照当前算法排序
    // 请自行补充……
    return 1;
}

void sorted_insert(struct free_block_type **head_ref, struct free_block_type *new_node) {
    struct free_block_type *current;

    // 特殊情况：链表为空或新节点应该插入到头部
    if (*head_ref == NULL || (*head_ref)->start_addr >= new_node->start_addr) {
        new_node->next = *head_ref;
        *head_ref = new_node;
    } else {
        // 找到新节点应该插入的位置
        current = *head_ref;
        while (current->next != NULL && current->next->start_addr < new_node->start_addr) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

void insertion_sort(struct free_block_type **head_ref) {
    struct free_block_type *sorted = NULL;
    struct free_block_type *current = *head_ref;

    while (current != NULL) {
        struct free_block_type *next = current->next;
        sorted_insert(&sorted, current);
        current = next;
    }

    *head_ref = sorted;
}

int dispose(struct allocated_block *free_ab) {
    struct allocated_block *pre, *ab;
    if (free_ab == allocated_block_head) { /*如果要释放第一个节点*/
        allocated_block_head = allocated_block_head->next;
        free(free_ab);
        return 1;
    }

    pre = allocated_block_head;
    ab  = allocated_block_head->next;
    while (ab != free_ab) { 
        pre = ab;
        ab  = ab->next; 
    }

    pre->next = ab->next;
    free(ab);
    return 2;
}

int display_mem_usage() {
    if (free_block == NULL) {
        printf("No memory is available.\n");
        return(-1);
    }

    struct free_block_type *fbt = free_block;
    struct allocated_block *ab  = allocated_block_head;

    printf("----------------------------------------------------------\n");
    /* 显示空闲区 */
    printf("Free Memory:\n");
    printf("%20s %20s\n", " start_addr", " size");
    while (fbt != NULL) {
        printf("%20d %20d\n", fbt->start_addr, fbt->size);
        fbt = fbt->next;
    }

    /* 显示已分配区 */
    printf("\nUsed Memory:\n");
    printf("%10s %20s %10s %10s\n", "PID", "ProcessName", "start_addr", " size");
    while (ab != NULL) {
        printf("%10d %20s %10d %10d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
        ab = ab->next;
    }
    printf("----------------------------------------------------------\n");
    return 0;
}

int do_exit() {
    struct free_block_type *fbt = free_block;
    struct allocated_block *ab  = allocated_block_head;
    struct free_block_type *pre = NULL;
    while (fbt != NULL) {
        pre = fbt;
        fbt = fbt->next;
        free(pre);
    }
    while (ab != NULL) {
        ab = ab->next;
        free(ab);
    }
    return 0;
}
struct allocated_block *find_process(int pid) {
    struct allocated_block *ab = allocated_block_head;
    while (ab != NULL) {
        if (ab->pid == pid) return ab;
        ab = ab->next;
    }
    return NULL;
}
