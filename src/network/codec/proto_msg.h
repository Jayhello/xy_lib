#pragma once
#include "xy_buffer.h"
#include <google/protobuf/message.h>
#include "comm/xy_exception.h"

namespace xy{

struct PbCodecException : public Exception {
    PbCodecException(const string &buffer) : Exception(buffer) {};
    PbCodecException(const string &buffer, int err) : Exception(buffer, err) {};
    ~PbCodecException() throw() {};
};

using std::string;
using ::google::protobuf::Message;
using ::google::protobuf::Descriptor;

void pbEncode(Message *msg, Buffer &buf);

// 成功的话返回pb接口体指针, 失败的抛出异常 PbCodecException
Message* decodePb(Buffer &buf);

} // xy
