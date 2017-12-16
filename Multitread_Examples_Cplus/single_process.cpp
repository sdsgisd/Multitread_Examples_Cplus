//
//  single_process.cpp
//  parallel_processing1
//
//  Created by Sadashige Ishida on 12/16/17.
//

#include <stdio.h>
#include "functions.h"

double compute_pi_single_process(const int num_of_partitions)
{
    if (num_of_partitions < 1) return -1.;
    
    const double width = 1. / (double)num_of_partitions;
    double height = 0.;
    double middle = 0.;
    double sum = 0.;
    double pi = 0.;
    int index = 0;
    
    for (index = 0; index < num_of_partitions; index++) {
        middle = (index + 0.5) * width;
        height = 4. / (1. + middle * middle);
        sum += height;
    }
    
    pi = width * sum;
    return pi;
}