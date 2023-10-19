#include "connection.h"
#include "acceptor.h"
#include "comm/logging.h"
#include "comm/comm.h"
#include "net_util.h"
#include "data_context.h"
#include "net_thread.h"

namespace xy{

Connection::Connection(int fd, AcceptorPtr pAcceptor, NetThread* pTh, const string& ip, uint16_t port, int timeout):
    _fd(fd), _pAcceptor(pAcceptor), _pTh(pTh), _ip(ip), _port(port), _timeoutSec(timeout){
}

Connection::~Connection(){
}

int Connection::recvData(){
    while(true){
        _recvBuffer.makeRoom();   // 保证有空余的数据
        int rd = ::read(_fd, _recvBuffer.end(), _recvBuffer.space());
        trace("read fd: %d, size: %d", _fd, rd);
        if(rd < 0 and errno == EINTR){
            continue;
        }else if (rd < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)){   // 读取完了
            break;
        }else if(rd <= 0){  // 读取出错了
            info("recv fd:%d fail, ret: %d", _fd, rd);
            return -1;
        }else{
            _recvBuffer.addSize(rd);
        }
    }

    Slice msg;
    int ret = _pAcceptor->getProtocol()(_recvBuffer, msg);
    trace("parse ret: %d, size: %d %s", ret, int(msg.size()), msg.data());

    if(0 == ret){       // 没有收到完整的包
        return 0;
    }else if(ret < 0){  // 解包失败(关闭链接)
        error("parse fd:%d fail, ret: %d", _fd, ret);
        return ret;
    }

    // 处理一个完整的包
    auto ptr = std::make_shared<RecvContext>(_fd, _ip, _port);
    ptr->buffer() = std::move(msg.toVecChar());

    _pAcceptor->pushRecvQueue(ptr);

    return 0;
}

void Connection::enableWrite(){
    _pTh->getEpoller()->mod(_fd, 0, EPOLLIN | EPOLLOUT);
    _bWriteEnabled = true;
}

void Connection::disableWrite(){
    _pTh->getEpoller()->mod(_fd, 0, EPOLLIN);
    _bWriteEnabled = false;
}

int Connection::sendData(const std::vector<char>& data){
    _sendBuffer.append(data.data(), data.size());
    return sendData();
}

int Connection::sendData(){
    while(not _sendBuffer.empty()){
        int rd = ::send(_fd, _sendBuffer.data(), _sendBuffer.size(), 0);
        if(rd < 0){
            if(errno == EINTR){
                continue;
            }else if(errno == EAGAIN || errno == EWOULDBLOCK){
                if(not _bWriteEnabled and (not _sendBuffer.empty())){
                    enableWrite();
                }
                break;
            }else{
                info("recv fd:%d fail, ret: %d", _fd, rd);
                return -1;
            }
        }else if(0 == rd){
            info("fd:%d send ret 0 close", _fd);
            return -1;
        }else{
            _sendBuffer.consume(rd);
            if(_sendBuffer.empty()){   // 数据发送完了
                if(_bWriteEnabled){
                    disableWrite();
                }
                break;
            }
        }
    }

    // 这里可以加些处理逻辑例如包积压太多, 关闭链接

    return 0;
}

// --------------------------------------------------------------------------------------------------------------

ConnectionManager::ConnectionManager(NetThread* pth):_pNetThread(pth){
}

ConnectionManager::~ConnectionManager(){
}

ConnectionPtr ConnectionManager::get(int fd){
    auto it = _mFdConPtr.find(fd);

    if(it != _mFdConPtr.end()){
        return it->second;
    }

    return nullptr;
}

void ConnectionManager::add(ConnectionPtr cPtr, long lTimeoutTs){
    _mFdConPtr[cPtr->fd()] = cPtr;
    _mTimeoutTsFd.insert(make_pair(lTimeoutTs, cPtr->fd()));
}

void ConnectionManager::del(ConnectionPtr cPtr){
    _mFdConPtr.erase(cPtr->fd());
}

void ConnectionManager::checkTimeout(time_t iCurTime){

}


} // xy
