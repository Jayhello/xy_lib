#pragma once

#include "xy_socket.h"
#include "handle.h"
#include <vector>
#include <memory>
#include "xy_codec.h"
#include "connection.h"

namespace xy{

class Acceptor{
public:
    // host填空则是 0.0.0.0
    int bind(short port, const std::string &host = "");

    int fd()const{return _sock.getfd();}

    int getTimeout()const{return _timeoutSec;}

    int setTimeout(int timeout){
        _timeoutSec = timeout;
    }

    void setProtocol(const ProtocolParserFunc& pf){
        _protocolParseFunc = pf;
    }

    ProtocolParserFunc& getProtocol(){
        return _protocolParseFunc;
    }

    void pushRecvQueue(const std::shared_ptr<SendContext>& context);

private:
    Ip4Addr                 _addr;
    Socket                  _sock;
    std::vector<Handler*>   _vHandle;
    int                     _timeoutSec;  // 链接多少秒后算超时

    ProtocolParserFunc      _protocolParseFunc;
};

using AcceptorPtr = Acceptor*;

} // xy
