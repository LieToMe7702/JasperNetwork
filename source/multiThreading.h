#pragma once
#include<functional>
#include <thread>
class MultiThreading{
    public:
    void CreateThread(std::function<void()> callback);
};