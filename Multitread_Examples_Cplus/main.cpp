//
//  main.cpp
//  parallel_processing1
//
//  Created by Sadashige Ishida on 12/16/17.
//
//  Reference (in Japanese): https://qiita.com/temoki/items/613f4ffa1d9bf751510a

#include <iostream>

#include "functions.h"

template<typename Func,typename ...Args>
double computational_time(Func& target_func,const unsigned iteration, Args ...args ){
    
    std::chrono::system_clock::time_point  start, end;
    start = std::chrono::system_clock::now();
    
    for(unsigned i=0;i<iteration;++i){
        target_func(args...);
    }
    
    end = std::chrono::system_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count(); //Convert time to ms.
    
    return elapsed;
    
}


int main(int argc, const char * argv[]) {
    using namespace std;
    
    const unsigned iteration_A=1;
    const unsigned partition_A=100000000;
    const unsigned iteration_B=1000;
    const unsigned partition_B=1000;
    
    const bool with_computational_result=false;
    
    cout<<"iteration="<<iteration_A<<", partition="<<partition_A<<endl;
    if(with_computational_result){
        cout<<"single process: "<<compute_pi_single_process(partition_A)<<endl;
        cout<<"pthread: "<<compute_pi_pthread(partition_A)<<endl;
        cout<<"cpp11: "<<compute_pi_cpp11_thread(partition_A)<<endl;
        cout<<"apple gcd: "<<compute_pi_apple_gcd(partition_A)<<endl;
        
    }
    cout<<"single process: "<<computational_time(compute_pi_single_process, iteration_A, partition_A)<<endl;
    cout<<"pthread: "<<computational_time(compute_pi_pthread, iteration_A, partition_A)<<endl;
    cout<<"cpp11: "<<computational_time(compute_pi_cpp11_thread, iteration_A, partition_A)<<endl;
    cout<<"apple gcd: "<<computational_time(compute_pi_apple_gcd, iteration_A, partition_A)<<endl;
    
    cout<<endl;
    
    cout<<"iteration="<<iteration_B<<", partition="<<partition_B<<endl;
    if(with_computational_result){
        cout<<"single process: "<<compute_pi_single_process(partition_B)<<endl;
        cout<<"pthread: "<<compute_pi_pthread(partition_B)<<endl;
        cout<<"cpp11: "<<compute_pi_cpp11_thread(partition_B)<<endl;
        cout<<"apple gcd: "<<compute_pi_apple_gcd(partition_B)<<endl;
    }
    cout<<"single process: "<<computational_time(compute_pi_single_process, iteration_B, partition_B)<<endl;
    cout<<"pthread: "<<computational_time(compute_pi_pthread, iteration_B, partition_B)<<endl;
    cout<<"cpp11: "<<computational_time(compute_pi_cpp11_thread, iteration_B, partition_B)<<endl;
    cout<<"apple gcd: "<<computational_time(compute_pi_apple_gcd, iteration_B, partition_B)<<endl;
  
    return 0;
}
