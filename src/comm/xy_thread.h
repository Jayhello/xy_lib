
#include "noncopyable.h"
#include "xy_exception.h"
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

    void start();

    virtual void run() = 0;

protected:

};

} // xy
