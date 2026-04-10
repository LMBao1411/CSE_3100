#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFERSIZE 13

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
    int items[5];
    
    while (1) {
        for (int i=0; i<5; i++) {
            items[i] = rand() % 100;
        }
        pthread_mutex_lock(&mutex);

        while (count + 5 > BUFFERSIZE) {
            printf("producer enters waiting while loop\n");
            pthread_cond_wait(&cond_producer, &mutex);
        }

        for (int i=0; i<5; i++) {
            buffer[count++] = items[i];
            printf("Produced: %d\n", items[i]);
        }

        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);
        sleep(3);
    }
}

void *consumer_func(void *arg) {
    int items[3];

    while (1) {
        pthread_mutex_lock(&mutex);

        while (count < 3) {
            printf("consumer enters waiting while loop\n");
            pthread_cond_wait(&cond_consumer, &mutex);
            printf("signal\n");
            
        }

        for (int i=0; i < 3; i++) {
            items[i] = buffer[count--];
            printf("Consumed: %d\n", items[i]);
        }

        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);
        sleep(2);
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