#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// We will use the 'n' provided by the user for the number of students
#define MAX_STUDENTS 100

typedef struct {
    int p;               // Number of consultations remaining to be performed
    int student_id;      // The ID of the student the professor is currently calling
    int ready;           // 0: currently no student in a consultation, 1: currently someone is talking
    pthread_mutex_t mutex;
    pthread_cond_t cond_student[MAX_STUDENTS]; 
    pthread_cond_t cond_prof;          
} data_t;

typedef struct {
    int id;        
    int *p_talked;       // Pointer to this student's consultation counter
    data_t *pdata; 
} thread_arg_t;

void *Student(void *t) {
    thread_arg_t *arg = t;
    int id = arg->id;
    int *p_talked = arg->p_talked;
    data_t *pdata = arg->pdata;

    while (1) {
        pthread_mutex_lock(&pdata->mutex);
        
        // PREDICATE: No student has been called. The Professor is free to pick a name.
        while((pdata->ready == 0 || pdata->student_id != id) && pdata->p > 0) {
            pthread_cond_wait(&pdata->cond_student[id], &pdata->mutex);
        }

        // EXIT CONDITION: No more consultations left and professor is done
        if (pdata->p <= 0 && pdata->ready == 0) {
            pthread_mutex_unlock(&pdata->mutex);
            break;
        }

        // CONSUME: If the professor called my ID
        if (pdata->ready && pdata->student_id == id) {
            printf("Student %d is talking to the Professor.\n", id);
            (*p_talked)++;
            pdata->ready = 0; // Finished talking
            pthread_cond_signal(&pdata->cond_prof);
        }
        
        pthread_mutex_unlock(&pdata->mutex);
    }
    pthread_exit(NULL);
}

void *Professor(void *t) {
    thread_arg_t *arg = t;
    data_t *pdata = arg->pdata;
    unsigned int s = (unsigned int)arg->id; // seed
    int num_students = arg->id; // repurposed arg to pass total N
    
    while (1) {
        pthread_mutex_lock(&pdata->mutex);

        // PREDICATE: A student has been called. The Professor must wait for them to finish.
        while (pdata->ready == 1) {
            pthread_cond_wait(&pdata->cond_prof, &pdata->mutex);
        }

        // EXIT CONDITION: Check if max consultations reached
        if (pdata->p <= 0) {
            // Wake up everyone so they can check the exit condition
            for (int i = 0; i < num_students; i++) {
                pthread_cond_broadcast(&pdata->cond_student[i]);
            }
            pthread_mutex_unlock(&pdata->mutex);
            break;
        }

        // PRODUCE: Pick a random student to talk to
        int next_id = rand_r(&s) % num_students;
        
        pdata->student_id = next_id;
        pdata->ready = 1;
        pdata->p--;

        printf("Professor calls Student %d (Consultations remaining: %d)\n", next_id, pdata->p);

        // Wake up the specific student called
        pthread_cond_signal(&pdata->cond_student[next_id]);

        pthread_mutex_unlock(&pdata->mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s [num_students] [max_consultations] [seed]\n", argv[0]);
        exit(0);
    }

    int n = atoi(argv[1]); // Number of students
    int p = atoi(argv[2]); // Max consultations
    int s = atoi(argv[3]); // Seed

    if (n > MAX_STUDENTS) n = MAX_STUDENTS;

    int talked_counts[MAX_STUDENTS] = {0};
    data_t data;
    pthread_t prof_thread;
    pthread_t student_threads[MAX_STUDENTS];
    thread_arg_t student_args[MAX_STUDENTS];
    thread_arg_t prof_arg;

    // Initialization
    pthread_mutex_init(&data.mutex, NULL);
    pthread_cond_init(&data.cond_prof, NULL);
    for (int i = 0; i < n; i++) {
        pthread_cond_init(&data.cond_student[i], NULL);
    }

    data.ready = 0;
    data.p = p;

    // Create Professor (Tosser logic)
    prof_arg.id = n; // Passing n here so professor knows the range for rand()
    prof_arg.pdata = &data;
    pthread_create(&prof_thread, NULL, Professor, &prof_arg);

    // Create Students (Counter logic)
    for (int i = 0; i < n; i++) {
        student_args[i].id = i;
        student_args[i].p_talked = &talked_counts[i];
        student_args[i].pdata = &data;
        pthread_create(&student_threads[i], NULL, Student, &student_args[i]);
    }

    // Join threads
    pthread_join(prof_thread, NULL);
    for (int i = 0; i < n; i++) {
        pthread_join(student_threads[i], NULL);
    }

    // Cleanup
    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond_prof);
    for (int i = 0; i < n; i++) {
        pthread_cond_destroy(&data.cond_student[i]);
    }

    for (int i = 0; i < n; i++) {
        printf("Student %d talked to professor %d times.\n", i, talked_counts[i]);
    }

    return 0;
}