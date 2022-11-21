#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int num_barrier;
    sem_t *sem_barrier;
    pthread_mutex_t *mutex;
} args;

void *thread_barreira(void *arg) {
    args *thread_args = (args *)arg;
    printf("Antes da barreira\n");

    pthread_mutex_lock(thread_args->mutex);
    if (thread_args->num_barrier < 3) {
        thread_args->num_barrier++;
        pthread_mutex_unlock(thread_args->mutex);
        sem_wait(thread_args->sem_barrier);
    } else {
        for (int i = 0; i < 4; i++)
            sem_post(thread_args->sem_barrier);
        pthread_mutex_unlock(thread_args->mutex);
    }

    printf("Depois da barreira\n");

    return NULL;
}

pthread_t *criar_threads(int n, args *arg) {
    pthread_t *tids = malloc(sizeof(pthread_t) * n);

    for (int i = 0; i < n; i++) {
        pthread_create(&tids[i], NULL, thread_barreira, arg);
    }

    return tids;
}

int main() {
    args threads_args;
    sem_t semaforo;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    sem_init(&semaforo, 0, 0);
    threads_args.sem_barrier = &semaforo;
    threads_args.num_barrier = 0;
    threads_args.mutex = &mutex;
    pthread_t *tids = criar_threads(4, &threads_args);

    for (int i = 0; i < 4; i++) {
        pthread_join(tids[i], NULL);
    }

    sem_destroy(&semaforo);

    return 0;
}
