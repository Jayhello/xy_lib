#pragma once
#include "comm/xy_thread.h"
#include "xy_epoll.h"
#include "connection.h"

namespace xy{

class NetThread : public Thread{
public:
    NetThread();

    ~NetThread();

    void addConnection(ConnectionPtr);

    virtual void run()override;



private:
    Epoller         _ep;

};

} // xy
