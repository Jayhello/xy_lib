#include "connection.h"
#include "acceptor.h"
#include "comm/logging.h"
#include "comm/comm.h"
#include "net_util.h"

namespace xy{

Connection::Connection(int fd, AcceptorPtr pAcceptor, NetThread* pTh, const string& ip, uint16_t port, int timeout):
    _fd(fd), _pAcceptor(pAcceptor), _pTh(pTh), _ip(ip), _port(port), _timeoutSec(timeout){
}

Connection::~Connection(){
}

int Connection::recv(){
    while(true){
        _recvBuffer.makeRoom();   // 保证有空余的数据
        int rd = ::read(_fd, _recvBuffer.end(), _recvBuffer.space());
        if(rd < 0 and errno == EINTR){
            continue;
        }else if (rd < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)){   // 读取完了
            break;
        }else if(rd <= 0){  // 读取出错了
            error("recv fd:%d fail, ret: %d", _fd, rd);
            return -1;
        }else{
            _recvBuffer.addSize(rd);
        }
    }

    Slice msg;
    int ret = _pAcceptor->getProtocol()(_recvBuffer, msg);

    if(0 == ret){       // 没有收到完整的包
        return 0;
    }else if(ret < 0){  // 解包失败(关闭链接)
        error("parse fd:%d fail, ret: %d", _fd, ret);
        return ret;
    }

    // 处理一个完整的包
    auto ptr = std::make_shared<SendContext>(_fd, _ip, _port);
    ptr->buffer() = std::move(msg.toVecChar());

    _pAcceptor->pushRecvQueue(ptr);

    return 0;
}

// --------------------------------------------------------------------------------------------------------------

ConnectionList::ConnectionList(NetThread* pth):_pNetThread(pth){
}

ConnectionList::~ConnectionList(){
}

ConnectionPtr ConnectionList::get(int fd){
    auto it = _mFdConPtr.find(fd);

    if(it != _mFdConPtr.end()){
        return it->second;
    }

    return nullptr;
}

void ConnectionList::add(ConnectionPtr cPtr, long lTimeoutTs){
    _mFdConPtr[cPtr->fd()] = cPtr;
    _mTimeoutTsFd.insert(make_pair(lTimeoutTs, cPtr->fd()));
}

void ConnectionList::checkTimeout(time_t iCurTime){

}


} // xy
