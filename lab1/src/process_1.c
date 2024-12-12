#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

pid_t wait(int *wstatus);

int main(int argc, char *argv[]) {
    pid_t pid, pid1;
    
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed.");
        return 1;
    } else if (pid ==0) {
        pid1 = getpid();
        printf("child: pid = %d\n", pid);
        printf("child: pid1 = %d\n", pid1);
    } else {
        pid1 = getpid();
        printf("parent: pid = %d\n", pid);
        printf("parent: pid1 = %d\n", pid1);

#ifndef STAGE_2
        wait(NULL);
#endif

        printf("\n");
    }

    return 0;
}