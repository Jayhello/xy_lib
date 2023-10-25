#pragma once

#include "network/basic/xy_socket.h"
#include "handle.h"
#include <vector>
#include <memory>
#include "network/codec/xy_codec.h"
#include "net_comm.h"
#include "comm/block_queue.h"
#include "comm/xy_monitor.h"
#include "comm/xy_exception.h"

namespace xy{

class Acceptor{
public:
    Acceptor(ServerPtr pServer);

    ~Acceptor();

    // 初始化scoket, host填空则是 0.0.0.0
    int init(short port, const std::string &host = "");

    int fd()const{return _sock.getfd();}

    int getTimeout()const{return _timeoutSec;}

    void setTimeout(int timeout){
        _timeoutSec = timeout;
    }

    void setProtocol(const ProtocolParserFunc& pf){
        _protocolParseFunc = pf;
    }

    ProtocolParserFunc& getProtocol(){
        return _protocolParseFunc;
    }

    template<typename CodecType>
    void setCodec(){
        _pCodec = new CodecType();
//        _protocolParseFunc = std::bind(&CodecType::tryDecode, _pCodec, std::placeholders::_1, std::placeholders::_2);
        _protocolParseFunc = [=](Slice data, Slice &msg){
            return _pCodec->tryDecode(data, msg);
        };
    }

    ServerPtr getServer(){return _pServer;}

    template<typename HandlerType, typename... Args>
    void setHandle(size_t num, Args&& ... args){
        if(not _vHandle.empty()){
            throw Exception("[Acceptor] has set handle already!!!!");
        }

        _iHandleNum = num;
        _vHandle.reserve(num);
        for(size_t i = 0; i < num; ++i){
            HandlerPtr ptr = new HandlerType(args...);

            ptr->setIndex(i);
            ptr->setAcceptor(this);
            ptr->setServer(getServer());

            _vHandle.push_back(ptr);
        }

        _vRecvData.reserve(num);
        for(size_t i = 0; i < num; ++i){
            _vRecvData.push_back(std::make_shared<RecvData>());
        }
    }

    HandlerPtr getHandle(int fd);

    const std::vector<HandlerPtr>& getHandles(){
        return _vHandle;
    }

protected:
    friend class Handler;
    friend class Connection;

    void pushRecvQueue(const std::shared_ptr<RecvContext>& context);

    // 返回空指针的话, 说明超时了
    std::shared_ptr<RecvContext> popRecvQueue(size_t idx, int timeoutMs);

private:
    using RecvQueue = BlockQueue<std::shared_ptr<RecvContext>>;
    struct RecvData{
        RecvQueue   _recvQueue;
//        Monitor     _monitor;
    };

private:
    ServerPtr                           _pServer;
    Ip4Addr                             _addr;
    Socket                              _sock;
    std::vector<HandlerPtr>             _vHandle;
    size_t                              _iHandleNum;
    vector<std::shared_ptr<RecvData>>   _vRecvData;
    int                                 _timeoutSec = 0;  // 链接多少秒后算超时

    ProtocolParserFunc                  _protocolParseFunc;
    CodecBase*                          _pCodec;
};

using AcceptorPtr = Acceptor*;

} // xy
