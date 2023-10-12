#pragma once
#include "xy_epoll.h"
#include "acceptor.h"
#include <vector>
#include <map>
#include "net_thread.h"
#include "net_comm.h"

namespace xy{

class Server{
public:
    Server();

    ~Server();

    int init();

    void addAcceptor(AcceptorPtr ptr);

    void waitForShutdown();

    void terminate();

    void sendResp(const std::shared_ptr<SendContext>& ctx);

    void close(const std::shared_ptr<SendContext>& ctx);

protected:
    int accept(int fd);

    NetThread* getNetThread(int cfd)const{
        return _vNetThread[getNetThreadIdx(cfd)];
    }

    int getNetThreadIdx(int cfd)const{
        return cfd % _iThreadNum;
    }

private:
    void startThread();

    void stopThread();

private:
    Epoller                     _ep;
    volatile bool               _stop;
    int                         _iThreadNum;
    std::vector<AcceptorPtr>    _vAcceptor;
    std::map<int, AcceptorPtr>  _mFdAcceptor;

    std::vector<NetThread*>     _vNetThread;
};

} // xy
