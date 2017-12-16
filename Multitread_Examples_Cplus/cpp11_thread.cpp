//
//  cpp11_thread.cpp
//  parallel_processing1
//
//  Created by Sadashige Ishida on 12/16/17.
//

#include <thread>
#include <mutex>
#include <vector>

double compute_pi_cpp11_thread(const int num_of_partitions)
{
    if (num_of_partitions < 1) return -1.;
    
    using ptr_thread = std::unique_ptr<std::thread>;
    using vec_thread = std::vector<ptr_thread>;
    
    const double width = 1. / static_cast<double>(num_of_partitions);
    const unsigned int num_of_threads = std::thread::hardware_concurrency();
    
    
    // Create threads.
    std::mutex mutex;
    double sum = 0.;
    int begin = 0;
    int end = 0;
    vec_thread threads;
    for (unsigned int index = 0; index < num_of_threads; index++) {
        // Allocate tasks.
        begin = (index == 0)? 0 : end + 1;
        end = (index == num_of_partitions - 1)
        ? num_of_partitions - 1
        : (num_of_partitions / num_of_threads) * (index + 1);
        
        // Push back threads.
        threads.push_back(ptr_thread(std::make_unique<std::thread> (
                                                                    [&sum, &mutex, width, begin, end](){
                                                                        double partial_sum = 0.;
                                                                        for (int i = begin; i <= end; i++) {
                                                                            double middle = (i + 0.5) * width;
                                                                            double height = 4. / (1. + middle * middle);
                                                                            partial_sum += height;
                                                                        }
                                                                        mutex.lock();
                                                                        sum += partial_sum;
                                                                        mutex.unlock();
                                                                    })));
    }
    
    // Join threads.
    std::for_each(threads.begin(), threads.end(), [](const ptr_thread& thread){
        thread->join();
    });
    
    double pi = width * sum;
    return pi;
}

