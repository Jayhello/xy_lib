#pragma once
#include "comm/xy_thread.h"
#include "net_comm.h"

namespace xy{

class Handler : public Thread{
public:
    Handler() = default;

    ~Handler() = default;

    virtual void run() override;

    void setServer(ServerPtr ps){_pServer = ps;}

    void setAcceptor(AcceptorPtr pa){_pAcceptor = pa;}

    void setIndex(int idx){_iHandleIdx = idx;}

    void process(const std::shared_ptr<RecvContext>& ctx);

protected:
    virtual void handle(const std::shared_ptr<RecvContext>& ctx) = 0;

    virtual void handleOverload(const std::shared_ptr<RecvContext>& ctx);

    virtual void handleTimeout(const std::shared_ptr<RecvContext>& ctx);

    void sendResp(const std::shared_ptr<SendContext>& ctx);

    void close(const std::shared_ptr<RecvContext>& ctx);

private:
    ServerPtr     _pServer;
    AcceptorPtr   _pAcceptor;
    int           _iHandleIdx;
};

} // xy
