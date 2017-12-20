//
//  pthread.cpp
//
//  Created by Sadashige Ishida on 12/16/17.
//

#include <stdio.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _thread_func_param
{
    double              width;
    int                 begin;
    int                 end;
    double*             sum;
    pthread_mutex_t*    mutex;
} thread_func_param;


void* thread_func(void* arg)
{
    thread_func_param* param = (thread_func_param*)arg;
    int i = 0;
    double height = 0.;
    double middle = 0.;
    double sum = 0.;
    
    for (i = param->begin; i <= param->end; i++) {
        middle = (i + 0.5) * param->width;
        height = 4. / (1. + middle * middle);
        sum += height;
    }
    
    pthread_mutex_lock(param->mutex);
    *(param->sum) += sum;
    pthread_mutex_unlock(param->mutex);
    
    return NULL;
}

double compute_pi_pthread(const int num_of_partitions)
{
    if (num_of_partitions < 1) return -1.;
    
    // Set equal to the num of the processors.
    int num_of_threads = (int)sysconf(_SC_NPROCESSORS_ONLN);
    
    const double width = 1. / (double)num_of_partitions;
    int index = 0;
    thread_func_param* params = NULL;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_t* threads = NULL;
    double sum = 0.;
    double pi = 0.;
    
    pthread_mutex_init(&mutex, NULL);
    
    // Allocate tasks
    params = (thread_func_param*)malloc(sizeof(thread_func_param) * num_of_threads);
    for (index = 0; index < num_of_threads; index++) {
        params[index].width = width;
        params[index].sum = &sum;
        params[index].mutex = &mutex;
        params[index].begin = (index == 0)? 0 : params[index - 1].end + 1;
        params[index].end = (index == num_of_partitions - 1)
        ? num_of_partitions - 1
        : (num_of_partitions / num_of_threads) * (index + 1);
    }
    
    // Create threads.
    threads = (pthread_t*)malloc(sizeof(pthread_t) * num_of_threads);
    for (index = 0; index < num_of_threads; index++) {
        pthread_create(&threads[index], NULL, thread_func, (void*)&params[index]);
    }
    
    // Join threads.
    for (index = 0; index < num_of_threads; index++) {
        pthread_join(threads[index], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    free(threads);
    free(params);
    
    pi = width * sum;
    return pi;
}