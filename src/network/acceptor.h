
#pragma once
#include "xy_socket.h"

namespace xy{

class Acceptor{
public:
    // host填空则是 0.0.0.0
    int bind(short port, const std::string &host);

    int fd()const{return _sock.getfd();}

private:
    Ip4Addr  _addr;
    Socket   _sock;
};

using AcceptorPtr = Acceptor*;

} // xy
