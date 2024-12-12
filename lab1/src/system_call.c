#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc > 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }

    pid_t pid = getpid();
    printf("system_call: PID = %d\n", pid);
    return 0;
}
