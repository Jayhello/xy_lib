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

protected:
    virtual void handle() = 0;

    void sendResp(const std::shared_ptr<SendContext>& ctx);

    void close(const std::shared_ptr<SendContext>& ctx);

private:
    ServerPtr     _pServer;
    AcceptorPtr   _pAcceptor;
    int           _iHandleIdx;
};

} // xy
