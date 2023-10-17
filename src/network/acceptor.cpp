
#include "acceptor.h"
#include "connection.h"
#include "data_context.h"
#include "net_util.h"
#include "xy_server.h"

namespace xy{

Acceptor::Acceptor(ServerPtr pServer):_pServer(pServer){
}

Acceptor::~Acceptor(){
}

int Acceptor::init(short port, const std::string &host){
    _sock.createSocket();
    _sock.bind(host, port);
}

HandlerPtr Acceptor::getHandle(int fd){
    int idx = fd % _iHandleNum;
    return _vHandle[idx];
}

void Acceptor::pushRecvQueue(const std::shared_ptr<RecvContext>& context){
//    ++iRecvSize;    // 这里可以根据handle的处理情况, 做

    _vRecvData[context->fd() % _iHandleNum]->_recvQueue.push_back(context);
//    _vRecvData[context->fd() % _iHandleNum]->_monitor.notify_one();
//    auto ph = getHandle(context->fd());
}

std::shared_ptr<RecvContext> Acceptor::popRecvQueue(size_t idx, int timeoutMs){
    std::shared_ptr<RecvContext> ptr;
    _vRecvData[idx]->_recvQueue.pop_wait(&ptr, timeoutMs);
    return ptr;
}


} // xy
