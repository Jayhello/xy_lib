//
// Created by Administrator on 2023/9/25.
//
#pragma once
#include <mutex>
#include <condition_variable>

namespace xy{

class Monitor{
public:
    void lock();

    void unlock();

    void wait();

    bool wait(int ms);

    void notify_one();

    void notify_all();

private:
    std::mutex                      _mtx;
    std::condition_variable_any     _cv;
//    std::condition_variable       _cv;   // 这个只能wait unique_lock, 因此上面使用 any
};

} // xy
