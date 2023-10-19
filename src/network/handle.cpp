#include "handle.h"
#include "xy_server.h"
#include "acceptor.h"
#include "comm/logging.h"
#include "data_context.h"

namespace xy{

void Handler::run(){
    info("[Handler] start run");
    loop();   // 这里之所以定义了loop, 是为了如果业务自定义run的时候扩展性好点
}

void Handler::loop(){
    while(not _pServer->hasTerminate()){
        std::shared_ptr<RecvContext> ctx = _pAcceptor->popRecvQueue(_iHandleIdx, 3000);
        if(ctx){  // 取出了数据
            process(ctx);
        }
    }
}

void Handler::process(const std::shared_ptr<RecvContext>& ctx){
    try{
        // 这里后面在处理
//        if (data->isOverload()) {
//            handleOverload(data);
//        }
//        if (data->isTimeout()) {
//            handleTimeout(data);
//        }

        handle(ctx);
    }catch (const std::exception& ex){
        error("handle %d, ex: %s", ctx->fd(), ex.what());
        close(ctx);
    }
}

void Handler::handleOverload(const std::shared_ptr<RecvContext>& ctx){

}

void Handler::handleTimeout(const std::shared_ptr<RecvContext>& ctx){

}

void Handler::sendResp(const std::shared_ptr<SendContext>& ctx){
    _pServer->sendResp(ctx);
}

void Handler::close(const std::shared_ptr<RecvContext>& ctx){

}

} // xy