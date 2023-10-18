#include "demo_server.h"

#include "comm/logging.h"
#include "comm/comm.h"
#include "network/xy_server.h"
#include "network/handle.h"
#include "network/acceptor.h"
#include "network/data_context.h"

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
        Log << "recv: " << ctx->ip() << ":" << ctx->port()<< "recv_data: " << ctx->buffer().data();

        shared_ptr<SendContext> send = std::make_shared<SendContext>(ctx->fd(), ctx->ip(), ctx->port(), 's');
        send->buffer() = ctx->buffer();
        sendResp(send);
    }
};


void simple_server(){
    Server server(2);
    server.init();
    info("server init");

    Acceptor acceptor(&server);
    acceptor.init(8080);
    acceptor.setCodec<LengthCodec>();
    acceptor.setHandle<EchoHandler>(3);
    info("acceptor init");

    server.addAcceptor(&acceptor);

    info("wait for shutdown");
    server.waitForShutdown();
}


} // xy