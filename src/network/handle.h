#pragma once
#include "comm/xy_thread.h"

namespace xy{

class  Handler : public Thread{
public:
    Handler();

    ~Handler();

    virtual void run() override;

protected:
    virtual void handle() = 0;

};

} // xy
