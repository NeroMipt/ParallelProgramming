//
// Created by egor on 4/22/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>

#define NUM_THREADS 4

typedef struct _data_t{
    int tid;
    double stuff;
}_data_t;

void *thr_func(void *arg){
    _data_t *data = (_data_t*)arg;
    std::cout << "thread id: " << data->tid << std::endl;
    pthread_exit(NULL);
}

int main(){
    pthread_t thr[NUM_THREADS];
    int i, rc;
    _data_t  thr_data[NUM_THREADS];

    for(i = 0; i < NUM_THREADS; ++i)
    {
        thr_data[i].tid = i;
        if((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))){
            std::cout << "error!!!!\n";
            return EXIT_FAILURE;
        }
    }

    for(i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(thr[i], NULL);
    }

    return EXIT_SUCCESS;
}
