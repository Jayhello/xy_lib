//
// Created by wenwen on 2023/3/12.
//
#include "src/comm/noncopyable.h"
#include "src/comm/xy_exception.h"

namespace xy{

class Thread : public noncopyable{
public:

    void start();

    virtual void run() = 0;

protected:

};

} // xy
