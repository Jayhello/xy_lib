#pragma once
#include "xy_buffer.h"
#include <unordered_map>
#include <map>
#include <mutex>
#include <string>
#include <memory>

namespace xy{

class NetThread;
class Acceptor;
using AcceptorPtr = Acceptor*;

struct SendContext{
    SendContext(int fd, const std::string& ip, uint16_t port):_fd(fd), _ip(ip), _port(port){}

    int fd()const{return _fd;}

    const std::string& ip()const{return _ip;}

    uint16_t port()const{return _port;}

    std::vector<char>& buffer(){return _buf;}

    const std::vector<char>& buffer()const{return _buf;}

    int                 _fd;
    std::string         _ip;
    uint16_t            _port;
    std::vector<char>   _buf;
};

class Connection{
public:
    Connection(int fd, AcceptorPtr pAcceptor, NetThread* pTh, const std::string& ip, uint16_t port, int timeout);

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

protected:
//    void pushRecvQueue();

private:
    int                 _fd;
    AcceptorPtr         _pAcceptor;
    NetThread*          _pTh;
    std::string         _ip;
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
    NetThread*                               _pNetThread;
    std::unordered_map<int, ConnectionPtr>   _mFdConPtr;
    std::multimap<long, int>                 _mTimeoutTsFd;
    std::mutex                               _mtx;
    long                                     _iLastCheckTs;
};


} // xy
