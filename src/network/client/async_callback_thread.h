#pragma once
#include "comm/xy_thread.h"
#include "comm/block_queue.h"
#include "msg_context.h"

namespace xy{

class AsyncCallbackThread : public Thread{
public:


private:
    bool                                        _terminate = false;
    BlockQueue<std::shared_ptr<ReqMessagePtr>>  _msgQueue;
};

} // xy
