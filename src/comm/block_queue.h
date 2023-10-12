#pragma once
#include "noncopyable.h"
#include <mutex>
#include <list>
#include <condition_variable>

namespace xy{

template<typename T>
class BlockQueue : public noncopyable{
public:
//    static const int k_wait_infinite = std::numeric_limits<int>::max();

    bool push_back(const T& value);

    bool pop_wait(T* v);

    bool pop_wait(T* v, int ms);

    bool empty()const;

    size_t size()const;

    bool hasStop()const{return _stop;}

    void setStop();

private:
    std::list<T>                _que;
    mutable  std::mutex         _mtx;
    std::condition_variable     _cond;
    volatile bool               _stop = false;
};

template<typename T>
bool BlockQueue<T>::push_back(const T& value){
    if(_stop) return false;

    bool bNotice = _que.empty();
    
    std::unique_lock<std::mutex> lk(_mtx);
    _que.push_back(value);

    if(bNotice){
        _cond.notify_one();
    }
    
    return true;
}

template<typename T>
bool BlockQueue<T>::pop_wait(T* v){
    std::unique_lock<std::mutex> lk(_mtx);
    _cond.wait(lk, [&](){
        return (not _que.empty() or _stop);
    });

    if(_que.empty() or _stop) return false;

    *v = std::move(_que.front());
    _que.pop_front();

    return true;
}

template<typename T>
bool BlockQueue<T>::pop_wait(T* v, int ms){
    std::unique_lock<std::mutex> lk(_mtx);
    _cond.wait_for(lk, std::chrono::milliseconds(ms), [&](){
        return (not _que.empty() or _stop);
    });

    if(_que.empty() or _stop) return false;

    *v = std::move(_que.front());
    _que.pop_front();

    return true;
}

template<typename T>
void BlockQueue<T>::setStop(){
    _stop = true;
    std::unique_lock<std::mutex> lk(_mtx);
    _cond.notify_all();
}

template<typename T>
bool BlockQueue<T>::empty()const{
    std::unique_lock<std::mutex> lk(_mtx);
    return _que.empty();
}

template<typename T>
size_t BlockQueue<T>::size()const{
    std::unique_lock<std::mutex> lk(_mtx);
    return _que.size();
}

} // xy
