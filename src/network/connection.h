#pragma once
#include "acceptor.h"

namespace xy{

class NetThread;

class Connection{
public:
    Connection(int fd, AcceptorPtr pAcceptor, NetThread* pTh, const string& ip, uint16_t port);

    ~Connection();

    int fd()const{
        return _fd;
    }

private:
    int                 _fd;
    AcceptorPtr         _pAcceptor;
    NetThread*          _pTh;
    string              _ip;
    uint16_t            _port;
    volatile  uint32_t  _uid;
};

using ConnectionPtr = Connection*;

class ConnectionList{
public:

private:

};


} // xy
