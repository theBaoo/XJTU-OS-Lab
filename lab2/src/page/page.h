#ifndef PAGE_H
#define PAGE_H
/**
 * definations for data structures and algorithms used.
 */

#include <stdlib.h>
#include <stdio.h>

#define PP 60    /* process page needed   */
#define MP 20     /* memory page available */
#define SIZE 12  /* size of sequence      */
#define P_SIZE 6
#define P_NUM  6

typedef enum {
    IN,
    OUT
} MEMORY_STATE;

typedef enum {
    USED,
    UNUSED
} PAGE_STATE;

typedef enum {
    LRU,
    FIFO
} ALGORITHM;

extern ALGORITHM algorithm;

typedef struct Page{
    int id;
    PAGE_STATE   used;     /* for page in memory  */
    MEMORY_STATE inmemory; /* for page in process */
    int counter;           /* for LRU */
    struct Page *p;
} Page;

extern int  diseffect;
extern int  memory_used;
extern Page memory_page [MP];
extern Page process_page[PP];
extern int  sub_process_page[P_NUM][P_SIZE];
extern int  sequence[SIZE];

int memory_full();
Page *memory_get();
Page *memory_find_lru();
extern Page *(*memory_take_place)();

void generate_sequence();
void random_sequence();
void by_hand_sequence();
// read ./sequence
void read_file_sequence();
void init_memory();
void init_process();
void execute_algorithm();
void display_memory(int);

#endif