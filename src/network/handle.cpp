#include "handle.h"
#include "xy_server.h"
#include "acceptor.h"
#include "comm/logging.h"
#include "data_context.h"

namespace xy{

void Handler::run(){

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

}

void Handler::close(const std::shared_ptr<RecvContext>& ctx){

}

} // xy