#include "menu.h"

int main() {

    while (1) {
        display_menu();
        get_choice();
        wait_key();
    }
    
    return 0;
}