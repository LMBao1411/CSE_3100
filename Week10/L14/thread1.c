#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *random_num(void *max) {
    // Cast the void pointer back to an int pointer
    int *max_num = (int *)max;
    int result = rand() / *max_num;
    printf("random number is: %d\n", result);
    return NULL;
}

int main() {
    pthread_t thread1;
    srand(time(NULL));
    int random_val = rand() % 100 + 1;
    
    // Pass the ADDRESS of random_val
    pthread_create(&thread1, NULL, random_num, (void *)&random_val);
    pthread_join(thread1, NULL);
    printf("finished.\n");
    return 0;
}