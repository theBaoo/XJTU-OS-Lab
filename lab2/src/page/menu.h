#ifndef MENU_H
#define MENU_H

#include <stdio.h>

typedef enum {
    RANDOM,
    BY_HAND,
    READ_FILE
} SEQUENCE_TYPE;

typedef void (*MenuFunction)();
typedef struct {
    char *name;
    MenuFunction function;
} MenuItem;

extern SEQUENCE_TYPE sequence_type;

extern MenuItem menu[];
extern MenuItem sequence_items[];

void display_menu();
void get_choice();
void change_algorithm();
void change_sequence();
// wait a key to display menu
void wait_key();
void do_exit();

#endif