#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>

#define NUM_READERS 5
#define NUM_WRITERS 5
#define NUM_ITERATIONS 3

pthread_mutex_t mutex; // data structuture to implement mutual exclusion
pthread_cond_t readers_proceed; // data structure to implement conditional argument for readers proceeding
pthread_cond_t writers_proceed; //data structure to implement conditional argument for writers proceeding

int readers_count = 0; // number of readers
int writers_waiting = 0; // number of writers
int shared_variable = 0; // the shared global variable

void *reader(void *arg) {
    int id = *((int *)arg);

    for(int i = 0; i < NUM_ITERATIONS; i++){
        usleep(rand() % 100000); //random wait so that reads and writes do not occur all at once

        pthread_mutex_lock(&mutex);

        while(writers_waiting > 0){
            pthread_cond_wait(&readers_proceed, &mutex);
        }

        readers_count++;
        pthread_mutex_unlock(&mutex);

        //read the shared variable
        printf("Reader %d: Read value: %d, Readers present: %d\n", id, shared_variable, readers_count);

        pthread_mutex_lock(&mutex);
        readers_count--;

        if(readers_count == 0){
            pthread_cond_signal(&writers_proceed);
        }

        pthread_mutex_unlock(&mutex);
    }

    free(arg);
    return NULL;
}

void *writer(void *arg){
    int id = *((int *)arg);
    for (int i = 0; i < NUM_ITERATIONS; i++){
        usleep(rand() % 100000); //random wait so that reads and writes do not occur all at once

        pthread_mutex_lock(&mutex);

        writers_waiting++;

        while(readers_count > 0){
            pthread_cond_wait(&writers_proceed, &mutex);
        }

        writers_waiting--;

        //write to the shared variable
        shared_variable = rand() % 100;
        printf("Writer %d: Wrote value %d, Readers present: %d\n", id, shared_variable, readers_count);

        pthread_cond_broadcast(&readers_proceed);
        pthread_mutex_unlock(&mutex);
    }

    free(arg);
    return NULL;
}

int main(){
    srand(time(NULL));

    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&readers_proceed, NULL);
    pthread_cond_init(&writers_proceed, NULL);

    for(int i=0; i<NUM_READERS; i++){
        int *id = malloc(sizeof(int));
        *id = i+1;
        pthread_create(&readers[i], NULL, reader, id);
    }

    for(int i=0; i<NUM_WRITERS; i++){
        int *id = malloc(sizeof(int));
        *id = i+1;
        pthread_create(&writers[i], NULL, writer, id);
    }

    for(int i=0; i<NUM_READERS; i++){
        pthread_join(readers[i], NULL);
    }

    for(int i=0; i<NUM_WRITERS; i++){
        pthread_join(writers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&readers_proceed);
    pthread_cond_destroy(&writers_proceed);

    return 0;
}