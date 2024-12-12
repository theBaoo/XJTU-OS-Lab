#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

pid_t wait(int *wstatus);
int system(const char *command);

int main(int argc, char *argv[]) {
    pid_t pid, pid1;
    
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed.");
        return 1;
    } else if (pid ==0) {
        pid1 = getpid();
        printf("child: PID = %d\n", pid1);

#ifdef SYS
        system("./system_call");
#elif defined(EXEC)
        char *args[] = {"./system_call", NULL};
        execv("./system_call", args);
#endif

    } else {
        pid1 = getpid();
        printf("parent: PID = %d\n", pid1);

#ifndef STAGE_2
        wait(NULL);
#endif
        printf("\n");
    }

    return 0;
}