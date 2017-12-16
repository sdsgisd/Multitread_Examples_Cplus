//
//  functions.h
//  parallel_processing1
//
//  Created by Sadashige Ishida on 12/16/17.
//

#ifndef functions_h
#define functions_h

double compute_pi_single_process(const int num_of_partitions=1);
double compute_pi_pthread(const int num_of_partitions=1);
double compute_pi_cpp11_thread(const int num_of_partitions=1);
double compute_pi_apple_gcd(const int num_of_partitions=1);

#endif /* functions_h */
