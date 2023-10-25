#pragma once
#include "comm/xy_thread.h"
#include "network/basic/xy_epoll.h"
#include "network/server/connection.h"
#include <vector>
#include "comm/block_queue.h"

namespace xy{

class Server;

class NetThread : public Thread{
public:
    NetThread(Server* pServer, int threadIdx);

    ~NetThread();

    void addConnection(ConnectionPtr);

    virtual void run()override;

    void terminate();

    void sendResp(const std::shared_ptr<SendContext>& ctx);

    void close(const std::shared_ptr<SendContext>& ctx);

    void delConnection(ConnectionPtr);

protected:
    friend class Connection;
    Epoller* getEpoller(){
        return &_ep;
    }

protected:
    // 处理fd的读写事件
    void processNet(const epoll_event &ev);

    // 内部事件
    void processPipe();

    Connection *getConnectionPtr(int fd) { return _conManager.get(fd); }

    using SendQueue = BlockQueue<std::shared_ptr<SendContext>>;

private:
    Server*             _pServer;
    int                 _threadIdx;
    Epoller             _ep;
    EpollNotice         _noticer;
    bool                _bTerminate = false;
    ConnectionManager   _conManager;
    SendQueue           _sendQueue;
};

} // xy
