#include <stdio.h>
#include <stdlib.h>

#define FF 1
#define BF 2
#define WF 3

const int K = 1024;

#define set_memory(x)        fprintf(file, "1\n%d\n", (int)(x))
#define set_algorithm(x)     fprintf(file, "2\n%d\n", (int)(x))
#define new_process(x)       fprintf(file, "3\n%d\n", (int)(x)); display()
#define terminate_process(x) fprintf(file, "4\n%d\n", (int)(x)); display()
#define display()            fprintf(file, "5\n")
#define quit()               fprintf(file, "0\n")

void create_testbench(const char *filename, const int algorithm);

int main() {
    create_testbench("testbenchFF", FF);
    create_testbench("testbenchBF", BF);
    create_testbench("testbenchWF", WF);
    return 0;
}

void create_testbench(const char *filename, const int algorithm) {
    FILE *file = fopen(filename, "w");
    
    // set_memory(2 * K);
    set_algorithm(algorithm);
    display();

    new_process(80);
    new_process(50);
    new_process(150);
    new_process(60);

    quit();

    fclose(file);
