//
// Created by wenwen on 2023/3/12.
//
#include "noncopyable.h"
#include "xy_exception.h"
#include <thread>

namespace xy{



class Thread : public noncopyable{
public:

    void start();

    virtual void run() = 0;

protected:

};

} // xy
