#include <iostream>
#include <thread>

#include <atomic>

using namespace std;

class A{
public:
    int value;
    A():value(0){};
    A(int value):value(value){};
    void increment(){++value;}
};


int ite=100000;
template <typename T>
void f_nonatomic( T* a) {
    
    for(int i=0;i<ite;++i){
        ++(a->value);
    }
    
}

template <typename T>
void f_atomic( atomic<T> *a) {
    
    for(int i=0;i<ite;++i){
        
        A temp=a->load();
        A newA;
        do{
            newA=temp;
            
            newA.value++;
            
        }while(!a->compare_exchange_weak(temp,newA));
        
    }
    
    
}



void thread_test(){
    A nonatomic_obj(0);
    atomic<A> atomic_obj(0);
    
    //thread thr_f1(f1,3);
    //thread thr_nonatomic(f_nonatomic, &nonatomic_obj);
    
    //Template function will not compile when called as a thread
    //https://stackoverflow.com/questions/8342839/template-function-will-not-compile-when-called-as-a-thread
    //https://stackoverflow.com/questions/30761225/how-to-use-stdatomic
    
    thread th=thread(f_nonatomic<A>,&nonatomic_obj);
    f_nonatomic(&nonatomic_obj);

    th.join();
    
    
    th=thread(f_atomic<A>,&atomic_obj);
    f_atomic(&atomic_obj);
    
    th.join();
    
    
    cout<<"nonatomic:"<<nonatomic_obj.value<<endl;
    cout<<"atomic:"<<atomic_obj.load().value<<endl;
    

    
}


template <typename T>
void Threader(T & counter)    // take by reference!
{
    counter++;
}

int test_func()
{
    unsigned int counter = 100;
    std::thread one(Threader<unsigned int>,   // specify template *instance*
                    std::ref(counter) );      // pass variable as reference
    one.join();
    return counter;
}

int main() {
    
    thread_test();
    
    cout<<test_func();
    
    return 0;
}
