
#include "acceptor.h"
#include "connection.h"
#include "data_context.h"

namespace xy{

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

std::shared_ptr<RecvContext> Acceptor::popRecvQueue(int idx, int timeoutMs){
    std::shared_ptr<RecvContext> ptr;
    _vRecvData[idx]->_recvQueue.pop_wait(&ptr, timeoutMs);
    return ptr;
}


} // xy
