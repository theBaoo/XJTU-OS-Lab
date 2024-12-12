#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <unistd.h>

int counter = 0;
int operations_times = 500000;
sem_t sem;

int   system(const char *command);
void *thread_func_counter(void *arg);
void *thread_func_cnt_sem(void *arg);
void *thread_func_system (void *arg);
void *thread_func_execv  (void *arg);
void *(*thread_func)     (void *arg);


int main(int argc, char const *argv[])
{
    int arg1 = 1, arg2 = 2;
    pthread_t t1, t2;
    sem_init(&sem, 0, 1);

#ifdef        STAGE_1
    thread_func = thread_func_counter;
#elif defined(STAGE_2)
    thread_func = thread_func_cnt_sem;
#elif defined(STAGE_3)
    thread_func = thread_func_system;
#elif defined(STAGE_4)
    thread_func = thread_func_execv;
#endif

    pthread_create(&t1, NULL, thread_func, &arg1);
    pthread_create(&t2, NULL, thread_func, &arg2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Counter value: %d\n", counter);
    sem_destroy(&sem);

    return 0;
}

void *thread_func_counter(void *arg) {
    (void)arg;
    for (int i = 0; i < operations_times; i++) {
        counter++;
    }
    return NULL;
}

void *thread_func_cnt_sem(void *arg) {
    (void)arg;
    for (int i = 0; i < operations_times; i++) {
        sem_wait(&sem);
        counter++;
        sem_post(&sem);
    }
    return NULL;
}

void *thread_func_system (void *arg) {
    // printf("thread%d tid = %lu, pid = %d\n", *(int *)arg, (unsigned long)pthread_self(), getpid());
    printf("thread%d tid = %ld, pid = %d\n", *(int *)arg, syscall(SYS_gettid), getpid());


    system("./system_call");
    printf("thread%d systemcall return\n", *(int *)arg);

    return NULL;
}

void *thread_func_execv  (void *arg) {
    // printf("thread%d tid = %ld, pid = %d\n", *(int *)arg, pthread_self(), getpid());
    printf("thread%d tid = %ld, pid = %d\n", *(int *)arg, syscall(SYS_gettid), getpid());

    char *args[] = {"/usr/bin/zsh", "-c", "cal", NULL};
    execv(args[0], args);

    return NULL;
}