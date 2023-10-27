#pragma once
#include "network/codec/xy_buffer.h"

namespace xy{

// tcp, udp传输的基类
class SocketBase{
public:
    SocketBase();

    ~SocketBase();

    void sendRequest();

protected:
    int                 _fd;
    Buffer              _recvBuffer;
    Buffer              _sendBuffer;
};

class TcpSocket : public SocketBase{
public:

};

class UdpSocket : public SocketBase{
public:

};

} // xy
