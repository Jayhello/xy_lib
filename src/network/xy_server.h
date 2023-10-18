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
    Server(int iThreadNum = 1);

    ~Server();

    int init();

    void addAcceptor(AcceptorPtr ptr);

    void waitForShutdown();

    // 停止服务
    void terminate();

    bool hasTerminate()const{return _stop;}

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
    void startNetThread();

    void stopNetThread();

    void startAcceptorHandles();

    void stopAcceptorHandles();

private:
    Epoller                     _ep;
    volatile bool               _stop;
    int                         _iThreadNum;
    std::vector<AcceptorPtr>    _vAcceptor;
    std::map<int, AcceptorPtr>  _mFdAcceptor;

    std::vector<NetThread*>     _vNetThread;
};

} // xy
