#include "proto_msg.h"
#include "comm/logging.h"
#include <google/protobuf/descriptor.h>
#include <string>
#include "comm/comm.h"

namespace xy{

using ::google::protobuf::DescriptorPool;
using ::google::protobuf::MessageFactory;

void pbEncode(Message *msg, Buffer &buf){
    size_t offset = buf.size();
    buf.appendValue((uint32_t) 0);
    const string &typeName = msg->GetDescriptor()->full_name();
    buf.appendValue((uint32_t) typeName.size());            // decode的时候, 反序列化为对应的pb
    buf.append(typeName.data(), typeName.size());
    msg->SerializeToArray(buf.allocRoom(msg->ByteSize()), msg->ByteSize());
    *(uint32_t *) (buf.begin() + offset) = buf.size() - offset;
}

// 成功的话返回pb接口体指针, 失败的话返回 nullptr
Message* decodePb(Buffer &buf){
    if (buf.size() < 8) {
        string tips = format("buffer is too small size: %lu", buf.size());
        throw PbCodecException(tips);
//        return NULL;
    }
    char *p = buf.data();
    uint32_t msglen = *(uint32_t *) p;
    uint32_t namelen = *(uint32_t *) (p + 4);
    if (buf.size() < msglen || buf.size() < 4 + namelen) {
        string tips = format("buf format error size %lu msglen %d namelen %d", buf.size(), msglen, namelen);
        throw PbCodecException(tips);
    }
    string typeName(p + 8, namelen);
    Message *msg = NULL;
    const Descriptor *des = DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
    if (des) {
        const Message *proto = MessageFactory::generated_factory()->GetPrototype(des);
        if (proto) {
            msg = proto->New();
        }
    }
    if (msg == NULL) {
        string tips = format("cannot create Message for name %s", typeName.c_str());
        throw PbCodecException(tips);
    }
    int r = msg->ParseFromArray(p + 8 + namelen, msglen - 8 - namelen);
    if (!r) {
        string tips = format("protobuf msg parse fail");
        delete msg;
        throw PbCodecException(tips);
    }
    buf.consume(msglen);
    return msg;
}

} // xy
