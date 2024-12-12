#include "page.h"
#include "queue.h"
#include "menu.h"
#include <unistd.h>
#include <time.h>

ALGORITHM algorithm = LRU;
SEQUENCE_TYPE sequence_type = READ_FILE;
Page *(*memory_take_place)() = memory_find_lru;
Page process_page[PP];
Page memory_page[MP];
int sub_process_page[P_NUM][P_SIZE];
int diseffect;
int memory_used;
int sequence[SIZE];

MenuItem sequence_items[] = {
    {"Random", random_sequence},
    {"By hand", by_hand_sequence},
    {"Read a file", read_file_sequence},
    {NULL, NULL}
};

int memory_full() {
    return memory_used == MP;
}

Page *memory_get() {
    for (int i = 0; i < MP; i++) {
        if (memory_page[i].used == UNUSED) {
            return &memory_page[i];
        }
    }
    return NULL;
}

Page *memory_find_lru() {
    Page *p = &memory_page[0];
    for (int i = 1; i < MP; i++) {
        if (memory_page[i].counter >= p->counter) continue;
        p = &memory_page[i];
    }
    return p;
}

void random_sequence() {
    srand(time(NULL));
    
    for (int i = 0; i < P_NUM; ++i) {
        for (int j = 0; j < P_SIZE; ++j) {
            sub_process_page[i][j] = rand() % PP;
        }
    }

    int i;
    for (i = 0; i < SIZE / P_SIZE; i++) {
        int p = rand() % P_NUM;
        for (int j = 0; j < P_SIZE; j++) {
            sequence[i * P_SIZE + j] = sub_process_page[p][j];
        }
        // for (int j = 0; j < P_NUM; j++) {
        //     for (int k = 0; k < P_SIZE; k++) {
        //         sequence[i * P_NUM * P_SIZE + j * P_SIZE + k] = sub_process_page[j][k];
        //     }
        // }
    }
    while (i < SIZE) {
        sequence[i++] = rand() % PP;
    }
    // int locality_window = MP / 3; // 局部性窗口大小

    // for (int i = 0; i < SIZE; i++) {
    //     if (i % locality_window == 0) {
    //         // 每隔 locality_window 个元素，随机选择一个新的局部性中心
    //         int locality_center = rand() % PP;
    //         for (int j = 0; j < locality_window && i + j < SIZE; j++) {
    //             // 在局部性中心附近生成页面ID
    //             sequence[i + j] = locality_center + (rand() % (locality_window / 2)) - 2;
    //             if (sequence[i + j] < 0) sequence[i + j] = 0;
    //             if (sequence[i + j] >= PP) sequence[i + j] = PP - 1;
    //         }
    //     }
    // }

    FILE *ofile = fopen("sequence.txt", "w"); 
    for (int i = 0; i < SIZE; i++) {
        fprintf(ofile, "%d\n", sequence[i]);
    }
    fclose(ofile);
}

void by_hand_sequence() {
    for (int i = 0; i < SIZE; i++) {
            printf("Please input the %dth page id: ", i);
            scanf("%d", &sequence[i]);
    }
}

void read_file_sequence() {
    FILE *ifile = fopen("sequence", "r");
    for (int i = 0; i < SIZE; i++) {
        fscanf(ifile, "%d", &sequence[i]);
    }
    fclose(ifile);
}

void generate_sequence() {
    sequence_items[sequence_type].function();

    printf("The sequence is: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", sequence[i]);
    }
    printf("\n");
}

void init_memory() {
    memory_used = 0;
    for (int i = 0; i < MP; i++) {
        memory_page[i].id = i;
        memory_page[i].used = UNUSED;
        memory_page[i].inmemory = OUT;
        memory_page[i].p = NULL;
    }
}

void init_process() {
    for (int i = 0; i < PP; i++) {
        process_page[i].id = i;
        process_page[i].used = UNUSED;
        process_page[i].inmemory = OUT;
        process_page[i].p = NULL;
    }
}

void execute_algorithm() {
    generate_sequence();
    init_memory();
    init_process();
    (void)createQueue();
    int flag = 0;
    diseffect = 0;

    for (int i = 0; i < SIZE; i++) {
        Page *page = &process_page[sequence[i]];
        if (page->inmemory == IN) {
            printf("Page %2d is already in memory. Hit!\n", page->id);
            page->p->counter = i; continue; 
        }

        ++diseffect;
        if (memory_full()) {
            flag = 1;
            Page *ptp = memory_take_place(); // memory page to be removed
            printf("Page %2d is replaced by Page %2d.\n", ptp->p->id, page->id);
            --memory_used;
            memory_page[ptp->id].counter = -1;
            memory_page[ptp->id].used = UNUSED;
            memory_page[ptp->id].p->p = NULL;
            memory_page[ptp->id].p->inmemory = OUT;
            memory_page[ptp->id].p = NULL;
            dequeue();
        }

        Page *p = memory_get();
        p->counter = i;
        p->used = USED;
        p->p = page;
        inqueue(p);
        page->inmemory = IN;
        page->p = p;
        ++memory_used;

        if (!flag) printf("Page %2d is loaded.\n", page->id);
        display_memory(p->id);
    }

    printf("The effect rate is: %d/%d, %f\n", SIZE - diseffect, SIZE, 1 - 1.0 * diseffect / SIZE);
}

void display_memory(int r) {
    // system("clear");
    // printf("\033[H");
    // printf("\033[20;0H");

    printf("-");
    for (int i = 0; i < MP; ++i) printf("---");
    printf("\n");

    for (int i = 0; i < MP; ++i) {
        printf("|%2d", i);
    }printf("|\n");

    printf("-");
    for (int i = 0; i < MP; ++i) printf("---");
    printf("\n");

    for (int i = 0; i < MP; ++i) {
        if (memory_page[i].id == r) {
            printf("|\033[31m%2d\033[0m", memory_page[i].p->id);
        } else if (memory_page[i].used == USED) {
            printf("|%2d", memory_page[i].p->id);
        } else {
            printf("|  ");
        }
    }printf("|\n");

    printf("-");
    for (int i = 0; i < MP; ++i) printf("---");
    printf("\n");
    usleep(10000);
}