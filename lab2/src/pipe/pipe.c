#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#define BUF_SIZE 4000
#define OP_TIMES 2000

#ifdef __LOCK
    #define LOCK   F_LOCK
#else
    #define LOCK   F_ULOCK
#endif

char c1 = 'A', c2 = 'B';
pid_t pid1, pid2;

void exit(int status);
pid_t waitpid(pid_t pid, int *status, int options);

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    int  fd[2];
    char buf[BUF_SIZE];
    pipe (fd);

    if ((pid1 = fork()) == -1) {
        perror("fork");
        return 1;
    } else if (pid1 == 0) {
        lockf(fd[1], LOCK, 0);

        buf[0] = c1;
        for (int i = 0; i < OP_TIMES; i++) {
            // sprintf(buf, c1);
            write(fd[1], buf, 1);
        }

        sleep(1);

        lockf(fd[1], F_ULOCK, 0);
        exit(0);
    }

    if ((pid2 = fork()) == -1) {
        perror("fork");
        return 1;
    } else if (pid2 == 0) {
        lockf(fd[1], LOCK, 0);

        buf[0] = c2;
        for (int i = 0; i < OP_TIMES; i++) {
            // sprintf(buf, c1);
            write(fd[1], buf, 1);
        }

        sleep(1);

        lockf(fd[1], F_ULOCK, 0);
        exit(0);
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    // 从管道中读4000个字符, 并添加到另一个缓冲区
    char buf2[BUF_SIZE + 1];
    read(fd[0], buf2, BUF_SIZE);
    buf2[BUF_SIZE] = '\0';
    printf("%s\n", buf2);

    return 0;
}
