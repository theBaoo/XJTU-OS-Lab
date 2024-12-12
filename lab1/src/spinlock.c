#include <stdio.h>
#include <pthread.h>

#define OP_TIMES 50000

struct spinlock_t {
    int flag;
};

void spinlock_init(struct spinlock_t *lock) {
    lock->flag = 0;
}

void spinlock_lock(struct spinlock_t *lock) {
    while (__sync_lock_test_and_set(&lock->flag, 1));
}

void spinlock_unlock(struct spinlock_t *lock) {
    __sync_lock_release(&lock->flag);
}

int shared_value = 0;

void *thread_func(void *arg) {
    struct spinlock_t *lock = (struct spinlock_t *)arg;
    (void)lock;
    for (int i = 0; i < OP_TIMES; i++) {
#ifdef SPIN_LOCK
        spinlock_lock(lock);
#endif
        shared_value++;
#ifdef SPIN_LOCK
        spinlock_unlock(lock);
#endif
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    struct spinlock_t lock;

    printf("Initial value: %d\n", shared_value);

    spinlock_init(&lock);

    pthread_create(&thread1, NULL, thread_func, &lock);
    pthread_create(&thread2, NULL, thread_func, &lock);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final   value: %d\n", shared_value);

    return 0;
}