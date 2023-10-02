#pragma once
#include "comm/xy_thread.h"
#include "xy_epoll.h"
#include "connection.h"
#include <vector>

namespace xy{

class Server;

class NetThread : public Thread{
public:
    NetThread(Server* pServer, int threadIdx);

    ~NetThread();

    void addConnection(ConnectionPtr);

    virtual void run()override;

    void terminate();

private:
    int             _threadIdx;
    Server*         _pServer;
    Epoller         _ep;
    EpollNotice     _noticer;
    bool            _bTerminate;
};

} // xy
