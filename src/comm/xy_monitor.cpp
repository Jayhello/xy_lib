//
// Created by Administrator on 2023/9/25.
//

#include "xy_monitor.h"

namespace xy{

void Monitor::lock(){
    _mtx.lock();
}

void Monitor::unlock(){
    _mtx.unlock();
}

void Monitor::wait(){
//    std::unique_lock<std::mutex> lk(_mtx);
    _cv.wait(_mtx);
}

bool Monitor::wait(int ms){
//    std::unique_lock<std::mutex> lk(_mtx);
    auto ret = _cv.wait_for(_mtx, std::chrono::milliseconds(ms));
    if(ret == std::cv_status::timeout){
        return false;
    }
    return true;
}

void Monitor::notify_one(){
    _cv.notify_one();
}

void Monitor::notify_all(){
    _cv.notify_all();
}

} // xy
