#pragma once
#include "xy_buffer.h"
#include <unordered_map>
#include <map>
#include <mutex>
#include <string>
#include <memory>
#include "net_comm.h"

namespace xy{

class Connection{
public:
    Connection(int fd, AcceptorPtr pAcceptor, NetThread* pTh, const std::string& ip, uint16_t port, int timeout);

    ~Connection();

    int fd()const{
        return _fd;
    }

    // ret < 0 接受错误
    int recvData();

    // 追加写数据
    int sendData(const std::vector<char>& data);

    // 发送, 或者写事件来了调用
    int sendData();

    int getTimeout()const{return _timeoutSec;}

    void setTimeout(int timeout){
        _timeoutSec = timeout;
    }

    bool writeEnabled()const{
        return _bWriteEnabled;
    }

    // 使得 fd 有写通知事件(一般不会有)
    void enableWrite();

    // 取消 fd 有写通知事件
    void disableWrite();

protected:
    friend class NetThread;

    Buffer& getSendBuffer(){
        return _sendBuffer;
    }

protected:
//    void pushRecvQueue();

private:
    int                 _fd;
    AcceptorPtr         _pAcceptor;
    NetThread*          _pTh;
    std::string         _ip;
    uint16_t            _port;
    bool                _bWriteEnabled;
    int                 _timeoutSec;  // 多少秒后算超时
    volatile  uint32_t  _uid;

    Buffer              _recvBuffer;
    Buffer              _sendBuffer;
};

using ConnectionPtr = Connection*;

class ConnectionManager{
public:
    ConnectionManager(NetThread*);

    ~ConnectionManager();

    ConnectionPtr get(int fd);

    void add(ConnectionPtr cPtr, long lTimeoutTs);

    void del(ConnectionPtr cPtr);

    void checkTimeout(time_t iCurTime);

private:
    NetThread*                               _pNetThread;
    std::unordered_map<int, ConnectionPtr>   _mFdConPtr;
    std::multimap<long, int>                 _mTimeoutTsFd;
    std::mutex                               _mtx;
    long                                     _iLastCheckTs;
};


} // xy
