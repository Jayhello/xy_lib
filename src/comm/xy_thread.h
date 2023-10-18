#pragma once

#include "noncopyable.h"
#include "xy_exception.h"
#include "xy_monitor.h"
#include <thread>

namespace xy{

class ThreadException : public Exception{
public:
    ThreadException(const string &buffer) : Exception(buffer) {};
    ~ThreadException(){};
};

class ThreadControl{
public:
    explicit ThreadControl(std::thread* th):_th(th){}

    // 等待当前线程结束, 不能在当前线程上调用
    void join();

    // detach, 不能在当前线程上调用
    void detach();

    // 交出当前线程运行权
    // std::this_thread::yield();

    std::thread* _th;
};

class Runnable{
public:
    virtual ~Runnable() = default;
    virtual void run() = 0;
};


class Thread : public Runnable , public noncopyable{
public:
    Thread():_running(false), _th(nullptr){}

    virtual ~Thread();

    ThreadControl start();

    virtual void run() = 0;

    bool isActive()const{return _running;}

    ThreadControl getThreadControl(){
        return ThreadControl(_th);
    }

protected:
    static void threadEntry(Thread *pThread);

protected:
    bool _running;

    std::thread  *_th;

    Monitor      _monitor;
};

} // xy
