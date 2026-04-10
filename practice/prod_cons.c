#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFERSIZE 12

int buffer[BUFFERSIZE];
int count = 0;      // number of items in the buffer

pthread_mutex_t mutex;
pthread_cond_t cond_consumer;
pthread_cond_t cond_producer;

void initialize_sync_objects() {
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("mutex init failed");
        exit(EXIT_FAILURE);
    }
    
    if (pthread_cond_init(&cond_consumer, NULL) != 0) {
        perror("consumer condition variable failed");
        exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(&cond_producer, NULL) != 0) {
        perror("producer condition variable failed");
        exit(EXIT_FAILURE);
    }
}

void cleanup_sync_objects() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_consumer);
    pthread_cond_destroy(&cond_producer);
}

void *producer_func(void *arg) {
    while (1) {
        // randomly produce between 2 and 5 items
        int n = rand() % 4 + 2;
        int items[6];

        for (int i = 0; i < n; i++) {
            items[i] = rand() % 100;
        }

        pthread_mutex_lock(&mutex);

        while (count + n > BUFFERSIZE) {
            printf("producer enters waiting while loop\n");
            pthread_cond_wait(&cond_producer, &mutex);
            printf("producer woke up\n");
        }

        for (int i = 0; i < n; i++) {
            buffer[count++] = items[i];
            printf("Produced: %d\n", items[i]);
        }

        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 4);
    }
}

void *consumer_func(void *arg) {
    while (1) {
        int n = rand() % 4 + 2;

        pthread_mutex_lock(&mutex);

        while (count < n) {
            printf("consumer enters waiting while loop\n");
            pthread_cond_wait(&cond_consumer, &mutex);
            printf("consumer woke up\n");
        }

        for (int i = 0; i < n; i++) {
            int item = buffer[count--];
            printf("Consumed: %d\n", item);
        }

        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3);
    }
}

int main() {
    pthread_t producer, consumer;

    initialize_sync_objects();

    pthread_create(&producer, NULL, producer_func, NULL);
    pthread_create(&consumer, NULL, consumer_func, NULL);
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    cleanup_sync_objects();
    return 0;
}