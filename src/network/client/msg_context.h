#pragma once

#include "common_pb/common.pb.h"
#include "comm/xy_monitor.h"
#include "network/codec/xy_buffer.h"
#include <memory>

namespace xy{

struct ReqMessage{
    ReqMessage() = default;
    ~ReqMessage(){

    }


    int             eStatus = EReqStatus_Doing;
    int             eType   = ECallType_Sync;
    RequestPacket   reqPackage;
    Buffer          sendBuffer;

    int64_t         iBeginTimeMs = 0;     // 请求时间(毫秒)(可以用于调用链路的耗时上报)
    int64_t         iEndTimeMs = 0;       // 完成时间


    Monitor*        ptrSyncMonitor = nullptr;     // 同步方式请求的时候用于等待响应
    std::shared_ptr<ResponsePacket> pResponse;    // 响应(不一定有因此用ptr, 例如本地队列里面就超时了)

};

using ReqMessagePtr = std::shared_ptr<ReqMessage>;

} // xy
