#include "multiThreading.h"

void MultiThreading::CreateThread(std::function<void()> callback){
    std::thread t1(callback);
    //t1.detach();
    
}
