#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>

void sigint_handle();
void sigalrm_handle();
void sigqit_handle();
void sigusr1_handle();
void sigusr2_handle();
void waiting();
void int_sigint();

int main()
{
    pid_t pid1 = -1, pid2 = -1;
    sem_t *sem1, *sem2;

    sem1 = sem_open("/sem_example", O_CREAT, 0644, 0);
    if (sem1 == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    sem2 = sem_open("/sem_example", O_CREAT, 0644, 0);
    if (sem2 == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    
    if ((pid1 = fork()) == -1) {
        perror("fork");
        exit(1);
    } else if (pid1 == 0) {
        signal(SIGUSR1, sigusr1_handle);
        signal(SIGINT, sigint_handle);
        sem_post(sem1);
        while (1)
            ;
        return 0;
    }

    if ((pid2 = fork()) == -1) {
        perror("fork");
        exit(1);
    } else if (pid2 == 0) {
        signal(SIGUSR2, sigusr2_handle);
        sem_post(sem2);
        while (1)
            ;
        return 0;
    }

    signal(SIGINT,  sigint_handle);
    signal(SIGQUIT, sigqit_handle);
    signal(SIGALRM, sigalrm_handle);

    alarm(5);
    sleep(10);
    
    sem_wait(sem1);
    sem_wait(sem2);
    printf("Parent process is sending signals to child processes\n");
    kill(pid1, SIGUSR1);
    kill(pid2, SIGUSR2);

    int status;
    if (waitpid(pid1, &status, 0) > 0) {
        if (WIFEXITED(status)) {
            printf("Child process1 exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process1 killed by signal %d\n", WTERMSIG(status));
        }
    } else {
        perror("waitpid");
    } 

    if (waitpid(pid2, &status, 0) > 0) {
        if (WIFEXITED(status)) {
            printf("Child process2 exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process2 killed by signal %d\n", WTERMSIG(status));
        }
    } else {
        perror("waitpid");
    } 

    printf("Parent process is killed\n");

    return 0;
}

void sigint_handle() {
    printf("PID %d: SIGINT is received.\n", getpid());
}

void sigqit_handle() {
    printf("PID %d: SIGQUIT is received.\n", getpid());
    // abort();
}

void sigalrm_handle() {
    printf("PID %d: SIGALRM is received.\n", getpid());
}

void sigusr1_handle() {
    printf("PID %d: SIGUSR1 is received.\n", getpid());
    exit(0);
}

void sigusr2_handle() {
    printf("PID %d: SIGUSR2 is received.\n", getpid());
    exit(0);
}

void int_sigint() {
    
}
