//
//  AppleGCD.cpp
//
//  Created by Sadashige Ishida on 12/16/17.
//

#include <stdio.h>
#include <dispatch/dispatch.h>
#include <stdlib.h>
#include <unistd.h>

double compute_pi_apple_gcd(const int num_of_partitions)
{
    if (num_of_partitions < 1) return -1.;
    
    // Set equal to the num of the processors.
    size_t num_of_threads = (size_t)sysconf(_SC_NPROCESSORS_ONLN);
    
    const double width = 1. / (double)num_of_partitions;
    size_t index = 0;
    size_t* begin = NULL;
    size_t* end = NULL;
    __block double sum = 0.;
    double pi = 0.;
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(1);
    
    //Allocate tasks.
    begin = (size_t*)malloc(sizeof(size_t) * num_of_threads);
    end = (size_t*)malloc(sizeof(size_t) * num_of_threads);
    for (index = 0; index < num_of_threads; index++) {
        begin[index] = (index == 0)? 0 : end[index - 1] + 1;
        end[index] = (index == num_of_partitions - 1)
        ? num_of_partitions - 1
        : (num_of_partitions / num_of_threads) * (index + 1);
    }
    
    //Start threads.
    dispatch_apply(num_of_threads,
                   dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
                   ^(size_t index){
                       size_t i = 0;
                       double height = 0.;
                       double middle = 0.;
                       double partional_sum = 0.;
                       
                       for (i= begin[index]; i <= end[index]; i++) {
                           middle = (i + 0.5) * width;
                           height = 4. / (1. + middle * middle);
                           partional_sum += height;
                       }
                       
                       dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
                       sum += partional_sum;
                       dispatch_semaphore_signal(semaphore);
                   });
    
    dispatch_release(semaphore);
    free(begin);
    free(end);
    
    pi = width * sum;
    return pi;
}