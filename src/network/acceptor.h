#pragma once

#include "xy_socket.h"
#include "handle.h"
#include <vector>
#include <memory>
#include "xy_codec.h"
#include "net_comm.h"
#include "comm/block_queue.h"
#include "comm/xy_monitor.h"

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

    void addHandle();

    HandlerPtr getHandle(int fd);

protected:
    friend class Handler;

    void pushRecvQueue(const std::shared_ptr<RecvContext>& context);

    // 返回空指针的话, 说明超时了
    std::shared_ptr<RecvContext> popRecvQueue(int idx, int timeoutMs);

private:
    using RecvQueue = BlockQueue<std::shared_ptr<RecvContext>>;
    struct RecvData{
        RecvQueue   _recvQueue;
//        Monitor     _monitor;
    };

private:
    Ip4Addr                             _addr;
    Socket                              _sock;
    std::vector<HandlerPtr>             _vHandle;
    size_t                              _iHandleNum;
    vector<std::shared_ptr<RecvData>>   _vRecvData;
    int                                 _timeoutSec = 0;  // 链接多少秒后算超时

    ProtocolParserFunc                  _protocolParseFunc;
};

using AcceptorPtr = Acceptor*;

} // xy
