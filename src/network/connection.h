#pragma once
#include "acceptor.h"
#include "xy_buffer.h"
#include <unordered_map>
#include <map>
#include <mutex>

namespace xy{

class NetThread;

class Connection{
public:
    Connection(int fd, AcceptorPtr pAcceptor, NetThread* pTh, const string& ip, uint16_t port, int timeout);

    ~Connection();

    int fd()const{
        return _fd;
    }

    // ret < 0 接受错误
    int recv();

    int getTimeout()const{return _timeoutSec;}

    int setTimeout(int timeout){
        _timeoutSec = timeout;
    }

private:
    int                 _fd;
    AcceptorPtr         _pAcceptor;
    NetThread*          _pTh;
    string              _ip;
    uint16_t            _port;
    int                 _timeoutSec;  // 多少秒后算超时
    volatile  uint32_t  _uid;

    Buffer              _recvBuffer;
    Buffer              _sendBuffer;
};

using ConnectionPtr = Connection*;

class ConnectionList{
public:
    ConnectionList(NetThread*);

    ~ConnectionList();

    ConnectionPtr get(int fd);

    void add(ConnectionPtr cPtr, long lTimeoutTs);

    void checkTimeout(time_t iCurTime);

private:
    NetThread*                          _pNetThread;
    unordered_map<int, ConnectionPtr>   _mFdConPtr;
    multimap<long, int>                 _mTimeoutTsFd;
    std::mutex                          _mtx;
    long                                _iLastCheckTs;
};


} // xy
