//https://stackoverflow.com/questions/13885617/how-to-use-stdatomic-effectively-for-non-primitive-types
#include <chrono>
#include <atomic>
#include <mutex>
#include <iostream>
#include <thread>

using namespace std;



class S {
public:
    int k = 0;
    int l = 0;
    S():k(0),l(0){};

};

S ordinary_s;

S mutex_s;
std::mutex _mux;

std::atomic<S> atomic_s;


void ordinary_increment() {
    ++(ordinary_s.k);
    ++(ordinary_s.l);
}



void mutex_increment() {
    std::lock_guard<std::mutex> lock(_mux);
    ++(mutex_s.k);
    ++(mutex_s.l);
}


void atomic_increment() {
    S tmp = atomic_s.load();
    S new_s;
    do {
        new_s = tmp;
        //new_s.doSomething(); // whatever modifications you want
        new_s.k++;
        new_s.l++;
    } while (!atomic_s.compare_exchange_strong(tmp, new_s));
    
}



auto exec_with_thread=[](auto f, int num ,auto... args){
    
    auto iterated_func=[=](const auto&... args){
        for(int i=0;i<num;++i){
            f(args...);
        }
    };

    thread th(iterated_func,args...);
    

    iterated_func(args...);
    th.join();
    //th.detach();
};

//https://cpprefjp.github.io/lang/cpp11/variadic_templates.html
template <class Func, class... Args>
double measure_timing(Func f, int ite,Args... args){
    std::chrono::high_resolution_clock clock;
    auto time = clock.now();
    for(int i=0;i<ite;++i){
        f(args...);
    }
    auto diff = clock.now() - time;
    
    return (double)diff.count() ;
}


#include <future>

auto exec_with_async=[](auto f, int num ,auto... args){
    
    auto iterated_func=[=](const auto&... args){
        for(int i=0;i<num;++i){
            f(args...);
        }
    };
    
    
    std::async(std::launch::async, iterated_func,args...);
    
    iterated_func(args...);
    
};

int main(void) {
    int ite=2;
    int num=10000;
    cout<<"[ordinary] time:"<<    measure_timing( exec_with_thread,ite,ordinary_increment,num)/ite<<", value:"<<ordinary_s.k<<endl;
    
    cout<<"[atomic] time:"<<    measure_timing( exec_with_thread,ite,atomic_increment,num)/ite<<", value:"<<atomic_s.load().k<<endl;
    cout<<"[mutex] time:"<<    measure_timing( exec_with_thread,ite,mutex_increment,num)/ite<<", value:"<<mutex_s.k<<endl;
    
    //std::future<void> f1 = std::async(std::launch::async, ordinary_increment);
    //f1.get();
    
    ordinary_s=S();
    
    //asyncでは保証されない。
    cout<<"[async oridinary] time:"<<        measure_timing( exec_with_async,ite,ordinary_increment,num)/ite<<", value:"<<ordinary_s.k<<endl;
    
    atomic_s.store(S());
    
    cout<<"[async atomic] time:"<<        measure_timing( exec_with_async,ite,atomic_increment,num)/ite<<", value:"<<atomic_s.load().k<<endl;
    
    mutex_s=S();
    cout<<"[async mutex] time:"<<        measure_timing( exec_with_async,ite,mutex_increment,num)/ite<<", value:"<<mutex_s.k<<endl;
    
    return 10;
}

