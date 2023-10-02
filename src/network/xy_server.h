#pragma once
#include "xy_epoll.h"
#include "acceptor.h"
#include <vector>
#include <map>
#include "net_thread.h"

namespace xy{

class Server{
public:
    Server();

    ~Server();

    int init();

    void addAcceptor(AcceptorPtr ptr);

    void waitForShutdown();

    void terminate();

protected:
    int accept(int fd);

private:
    Epoller                     _ep;
    volatile bool               _stop;
    int                         _iThreadNum;
    std::vector<AcceptorPtr>    _vAcceptor;
    std::map<int, AcceptorPtr>  _mFdAcceptor;

    std::vector<NetThread*>     _vNetThread;
};

} // xy
