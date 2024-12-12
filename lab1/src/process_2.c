#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

pid_t wait(int *wstatus);

int counter = 0;

int main(int argc, char *argv[]) {
    pid_t pid;
    
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed.");
        return 1;
    } else if (pid ==0) {
        counter += 2024;
        printf("child: counter = %d\n", counter);
        printf("child: counter_addr = %p\n", &counter);
    } else {
        wait(NULL);
        counter -= 2024;
        printf("parent: counter = %d\n", counter);
        printf("parent: counter_addr = %p\n", &counter);
        // wait(NULL);
        printf("\n");
    }
#ifdef STAGE_4
    counter *= 2;
    printf("before return: counter = %d, counter_addr = %p\n", counter, &counter);
#endif
    return 0;
}