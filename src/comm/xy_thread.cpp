//
// Created by wenwen on 2023/3/12.
//

#include "xy_thread.h"

namespace xy{

Thread::~Thread(){
    if (nullptr != _th) {
        //如果资源没有被detach或者被join，则自己释放
        if (_th->joinable()) {
            _th->detach();
        }

        delete _th;
        _th = NULL;
    }
}

ThreadControl Thread::start(){
    std::unique_lock<Monitor> syn(_monitor);

    if(_running){
        throw ThreadException("thread has start");
    }

    try{
        _th = new std::thread(&Thread::threadEntry, this);
    }catch(...){
        throw ThreadException("thread start error");
    }

    _monitor.wait();

    return ThreadControl(_th);
}

void Thread::threadEntry(Thread *pThread){
    pThread->_running = true;

    {
        std::unique_lock<Monitor> syn(pThread->_monitor);
        pThread->_monitor.notify_all();
    }

    try {
        pThread->run();
    }catch (exception &ex) {
        pThread->_running = false;
        throw ex;
    }catch (...) {
        pThread->_running = false;
        throw;
    }
    pThread->_running = false;
}


} // xy