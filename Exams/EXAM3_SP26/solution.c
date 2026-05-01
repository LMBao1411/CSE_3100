#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#define NUM_CHAIRS 2
#define MAXSTUDENTS 10000
int chair[NUM_CHAIRS] = {0};

typedef struct {
    int p;                  // number of people prof can take in
    int student_id;         // student id
    int ready;              // 0 when chairs are empty; 1 when prof is busy
    int seed;               // seed
    pthread_mutex_t mutex;
    pthread_cond_t cond_student[MAXSTUDENTS];
    pthread_cond_t cond_prof;
} data_t;

typedef struct {
    int id;
    int *p_talked;
    data_t *pdata;
} thread_arg_t;


void *Professor(void* thread_arg) {
    thread_arg_t *arg = thread_arg;
    int id = arg->id;
    int *num_consultations = arg->p_talked;
    data_t *pdata = arg->pdata;

    while (1) {
        pthread_mutex_lock(&pdata->mutex);

        while (pdata->ready == 1 && chair[2] != 0 && chair[1] != 0) {                      // predicate: prof is busy or waiting chair is empty, prof needs to finish talking -> wait(cond_prof)
            pthread_cond_wait(&pdata->cond_prof, &pdata->mutex);
        }

        if (pdata->p == 0) {                                            // prof is done for the day, wake everyone up and they should all leave
            for (int i = 0; i < MAXSTUDENTS; i++) {
                pthread_cond_broadcast(&pdata->cond_student[i]);
            }
            pthread_mutex_unlock(&pdata->mutex);
            break;
        }
        
        // operations
        pdata->ready = 0;
        pdata->p--;
        num_consultations++;
        printf("Professor is talking to student %d, consultation %d\n", pdata->student_id, *num_consultations);
        sleep(3);
        printf("Professor is done talking to %d\n", pdata->student_id);

        // signal the next student
        int next_id = id + 1;
        pthread_cond_signal(&pdata->cond_student[next_id]);

        pthread_mutex_unlock(&pdata->mutex);
    }
    pthread_exit(NULL);
}

void *Student(void *thread_arg) {
    thread_arg_t *arg = thread_arg;
    data_t *pdata = arg->pdata;
    int student_id = arg->id;
    while (1) {
        sleep(rand() % 3 + 1);
        while ((pdata->ready == 0 || pdata->student_id != student_id) && (chair[1] != 1) ) {
            pthread_cond_wait(&pdata->cond_student[student_id], &pdata->mutex);
        }

        printf("Student %d knocks the door\n", student_id);

        if (student_id > pdata->p) {
            printf("Student %d leaves because no more consultations today\n", student_id);
            pthread_mutex_unlock(&pdata->mutex);
            break;
        }
        
        if (chair[1] != 0) {            // waiting chair is not empty so student go home
            printf("Student %d leaves because no chairs left to wait\n", student_id);
            pthread_mutex_unlock(&pdata->mutex);
            break;
        }

        if (pdata->ready && pdata->student_id == student_id) {
            pdata->ready = 0;
            pthread_cond_signal(&pdata->cond_prof);
        }        
        pthread_mutex_unlock(&pdata->mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);              // number of participating students
    int seed = atoi(argv[2]);           // seed
    int p = atoi(argv[3]);              // prof office hours capacity
    srand(seed);

    data_t data;
    pthread_t prof_thread;
    pthread_t student_threads[n];
    thread_arg_t student_args[n];
    thread_arg_t prof_arg;

    data.ready = 0;
    data.p = p;

    pthread_mutex_init(&data.mutex, NULL);
    pthread_cond_init(&data.cond_prof, NULL);
    for (int i = 0; i < n; i++) {
        pthread_cond_init(&data.cond_student[i], NULL);
    }

    prof_arg.id = n;
    prof_arg.pdata = &data;
    pthread_create(&prof_thread, NULL, Professor, &prof_arg);

    for (int i = 0; i < n; i++) {
        student_args[i].id = i;
        student_args[i].pdata = &data;
        pthread_create(&student_threads[i], NULL, Student, &student_args[i]);
    }

    pthread_join(prof_thread, NULL);
    for (int i = 0; i < n; i++) {
        pthread_join(student_threads[i], NULL);
    }

    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond_prof);
    for (int i = 0; i < n; i++) {
        pthread_cond_destroy(&data.cond_student[i]);
    }

    return 0;
}