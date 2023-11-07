#pragma once
#include "xy_buffer.h"
#include <google/protobuf/message.h>

namespace xy{

using ::google::protobuf::Message;
using ::google::protobuf::Descriptor;

void pbEncode(Message *msg, Buffer &buf);

// 成功的话返回pb接口体指针, 失败的话返回 nullptr
Message* decodePb(Buffer &buf);

} // xy
