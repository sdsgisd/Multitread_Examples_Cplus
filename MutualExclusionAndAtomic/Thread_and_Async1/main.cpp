//https://qiita.com/termoshtt/items/d3cb7fe226cdd498d2ef
#include <vector>
#include <future>
#include <iostream>
using namespace std;

int num=6;
int large_number=1000000;

void thread_version(){
    cout<<"thread version"<<endl;

    std::vector<int> values(num);
    std::vector<std::thread> threads;
    for (int i = 0; i < num; ++i) {
        threads.push_back(std::thread([i, &values] {
            
            values[i] = 0;
            for(int k=0;k<large_number;++k){
                for(int j=1;j<=i;++j){
                    values[i]+=i;
                }
                
            }
            
            
        }));
    }
    
    for (int i=0;i<num;++i) {
        std::cout << i<<", "<<values[i]-i*i*large_number << std::endl;
    }
    
    cout<<endl;
    
    for (std::thread &th : threads) {
        th.join();
    }
    
    for (int i=0;i<num;++i) {
        std::cout << i<<", "<<values[i]-i*i*large_number << std::endl;
    }
    
    cout<<endl;

}

void async_version(){
    cout<<"async version"<<endl;
    std::vector<std::future<int>> asyncs;
    for (int i = 0; i < num; ++i) {
        
        asyncs.push_back(std::async([i](){
            int result =0;
            for(int k=0;k<large_number;++k){
                for(int j=1;j<=i;++j){
                    result+=i;
                }
            }
            return result;
        }
                                    ));
    }
    
    //getはtask終了を待って変数をゲットする。
    for (int i=0;i<num;++i) {
        std::cout << i<<", "<<asyncs[i].get()-i*i*large_number << std::endl;
    }
  
}

int main(int argc, char const *argv[]) {

    thread_version();
    async_version();
    
    return 0;
}
