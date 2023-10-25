#include "demo_server.h"

#include "comm/logging.h"
#include "comm/comm.h"
#include "network/server/xy_server.h"
#include "network/server/data_context.h"

using namespace xy;

int main(){
    setloglevel(Logger::LTRACE);

    simple_server();

    return 0;
}


namespace xy{

class EchoHandler : public Handler{
public:

protected:
    virtual void handle(const std::shared_ptr<RecvContext>& ctx)override{
        ScopeLog Log;
        Log << "[EchoHandle] recv: " << ctx->ip() << ":" << ctx->port()<< ", recv_data: " << ctx->buffer().data();

        shared_ptr<SendContext> send = std::make_shared<SendContext>(ctx->fd(), ctx->ip(), ctx->port(), 's');
        send->buffer() = ctx->buffer();
        sendResp(send);
    }
};


void simple_server(){
    Server server(1);
    server.init();
    info("server init");

    Acceptor acceptor(&server);
    acceptor.init(8088, "127.0.0.1");
    acceptor.setCodec<LengthCodec>();
    acceptor.setHandle<EchoHandler>(3);
    info("acceptor init");

    server.addAcceptor(&acceptor);

    info("wait for shutdown");
    server.waitForShutdown();
}


} // xy
