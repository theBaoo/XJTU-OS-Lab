#include "menu.h"
#include "page.h"
#include "queue.h"

int working = 1;

MenuItem menu[] = {
    {"1. Change algorithm  \n", change_algorithm},
    {"2. Change sequence   \n", change_sequence},
    {"3. Execute algorithms\n", execute_algorithm},
    {"4. Compare algorithms\n", NULL},
    {"5. Exit              \n", do_exit},
    {NULL, NULL}
};

void display_menu() {
    printf("================================\n");
    for (int i = 0; menu[i].name != NULL; i++) {
        printf("%s", menu[i].name);
    }
    printf("================================\n");
}

void get_choice() {
    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > 5) { printf("Invalid choice\n"); return; }
    if (menu[choice - 1].function == NULL) return;
    menu[choice - 1].function();
}

void change_algorithm() {
    printf("Please select the algorithm you want to use:\n");
    printf("\t1. LRU\n");
    printf("\t2. FIFO\n");

    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            algorithm = LRU;
            memory_take_place = memory_find_lru;
            printf("LRU algorithm selected\n");
            break;
        case 2:
            algorithm = FIFO;
            memory_take_place = peek;
            printf("FIFO algorithm selected\n");
            break;
        default:
            printf("Invalid choice\n");
            break;
    }
}

void change_sequence() {
    printf("Please select the sequence you want to use:\n");
    printf("\t1. Random \n");
    printf("\t2. By-hand\n");
    printf("\t3. Read a file\n");

    int choice;
    scanf("%d", &choice);
    if (choice < 1 || choice > 3) { printf("Invalid choice\n"); return; }
    sequence_type = choice - 1;
    printf("%s selected.\n", sequence_items[choice - 1].name);
}

void wait_key() {
    printf("Press any key to continue...\n");
    // one to consume the newline character, one to wait a key
    getchar();
    getchar();
}

void do_exit() {
    // free memory
    exit(0);
}