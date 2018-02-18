//http://minus9d.hatenablog.com/entry/20130914/1379168684
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

// グローバル領域にmtxを用意
std::mutex mtx;

void non_thread_safe_worker() {
 
    // 資源を使った処理を行う
    std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
    
}

void mutex_worker() {
    // 共通の資源（ここでは標準出力）を使う前にロックをかける
    std::lock_guard<std::mutex> lock(mtx);
    
    //unique_lockだと後からlockを取得できる。
    //https://cpprefjp.github.io/reference/mutex/unique_lock.html
    //std::unique_lock<std::mutex> lock;
    //lock=std::unique_lock<std::mutex>(mtx);
    
    // 資源を使った処理を行う
    std::cout << "thread id: " << std::this_thread::get_id() << std::endl;
    
}

#include <future>

int main() {
    
    // スレッドを4つ作る
    std::vector<std::thread> ths(10);
    
    //Non-thread safe process.
    std::cout<<"Non-thread safe process."<<std::endl;
    for (auto& th : ths) {
        th = std::thread(non_thread_safe_worker);
    }
    
    // すべてのスレッドが終わるのを待つ
    for (auto& th : ths) {
        th.join();
    }
    
    std::cout<<std::endl<<"Thread safe process."<<std::endl;
    
    //Thread safe process.
    for (auto& th : ths) {
        th = std::thread(mutex_worker);
    }
    
    // すべてのスレッドが終わるのを待つ
    for (auto& th : ths) {
        th.join();
    }
    
    //Asyncはcoutではうまくいくが、実は値の整合性は保証されない。
    std::cout<<std::endl<<"Async Version. Non-thread safe process."<<std::endl;
    std::vector<std::future<void>> asyncs(4);
    for (auto& asy: asyncs) {
        asy=std::async(std::launch::async, non_thread_safe_worker);
    }
    
    for (auto& asy: asyncs) {
        asy.get();
    }

    
    return 0;
}
