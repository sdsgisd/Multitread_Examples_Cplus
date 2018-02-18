//http://d.hatena.ne.jp/fjnl/20111206/1323159773
#include <iostream>
#include <thread>
#include <future>

using namespace std;

void throw_exception() {
    throw 1;
}

void exception_failure(){
    std::thread thr;
    
    try {
        thr=thread(throw_exception);
    } catch (...) {
        std::cout << "catch!!!" << std::endl;
    }
    
    thr.join();
}

int f(int index,int x, int y) {
    for(int i=0;i<1000;++i){
        cout<<"func "<<index<<" executed"<<endl;
    }
    if (x < 0 || y < 0) throw "f";

    return 100;
}


void exception_success(){
    //例外はgetで捕捉されるが、関数の実行そのものはget要らない。
    std::future<int> f1 = std::async(std::launch::async, f,1, 100, 200);
    

    std::future<int> f2 = std::async(std::launch::async, f,2, 100, -200);
    
    std::cout << f1.get() << std::endl;

    try {
        std::cout << f2.get() << std::endl;
    } catch (...) {
        std::cout << "catch!!!" << std::endl;
    }
}

int main() {
    //exception_failure();
    exception_success();
    
    return 0;
}
