#pragma once

#include "common_pb/common.pb.h"


namespace xy{

struct ReqMessage{
    int             eStatus = EReqStatus_Doing;
    int             eType   = ECallType_Sync;
    RequestPacket   reqPackage;

    int64_t         iBeginTimeMs = 0;     // 请求时间(毫秒)(可以用于调用链路的耗时上报)
    int64_t         iEndTimeMs = 0;       // 完成时间
};

} // xy
