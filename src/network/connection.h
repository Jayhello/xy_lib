#pragma once
#include "acceptor.h"

namespace xy{

class Connection{
public:
    Connection(AcceptorPtr pAcceptor, int fd, const string& ip, uint16_t port);

    ~Connection();



private:
    AcceptorPtr         _pAcceptor;
    int                 _fd;
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
