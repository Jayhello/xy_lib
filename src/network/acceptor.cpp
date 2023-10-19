
#include "acceptor.h"
#include "connection.h"
#include "data_context.h"
#include "net_util.h"
#include "xy_server.h"
#include "comm/logging.h"

namespace xy{

Acceptor::Acceptor(ServerPtr pServer):_pServer(pServer){
}

Acceptor::~Acceptor(){
}

int Acceptor::init(short port, const std::string &host){
    _sock.createSocket();
    _sock.bind(host, port);
    _sock.listen(1024);

    return 0;   // 待后面加返回码做错误判断
}

HandlerPtr Acceptor::getHandle(int fd){
    int idx = fd % _iHandleNum;
    return _vHandle[idx];
}

void Acceptor::pushRecvQueue(const std::shared_ptr<RecvContext>& context){
//    ++iRecvSize;    // 这里可以根据handle的处理情况, 做
    int idx = context->fd() % _iHandleNum;
//    trace("idx: %d, push fd: %d, size: %d", idx, context->fd(), int(context->buffer().size()));
    _vRecvData[idx]->_recvQueue.push_back(context);
//    _vRecvData[context->fd() % _iHandleNum]->_monitor.notify_one();
//    auto ph = getHandle(context->fd());
}

std::shared_ptr<RecvContext> Acceptor::popRecvQueue(size_t idx, int timeoutMs){
    std::shared_ptr<RecvContext> ptr;
    bool res = _vRecvData[idx]->_recvQueue.pop_wait(&ptr, timeoutMs);
//    trace("pop: %d, res: %d", (!ptr.get() ? 0 : ptr->fd()), res);
    return ptr;
}


} // xy
