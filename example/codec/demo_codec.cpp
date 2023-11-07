#include "demo_codec.h"
#include "comm/logging.h"
#include "comm/comm.h"
#include "network/codec/xy_codec.h"
#include "network/codec/proto_msg.h"
#include "common_pb/busi_pb.pb.h"
#include <memory>

int main(int argc, char** argv){
//    xy::simple_codec();
    xy::simple_codec_bad();

    return 0;
}

namespace xy{

void simple_codec(){
    SimpleCodec sc1;
    sc1.set_id(1);
    sc1.set_name("xy1");

    Buffer buffer1;
    pbEncode(&sc1, buffer1);

    info("pb encode buf_size: %d", int(buffer1.size()));

    std::unique_ptr<SimpleCodec> psc;

    try{
        psc.reset(static_cast<SimpleCodec*>(decodePb(buffer1)));
    }catch(const std::exception& ex){
        error("parse pb fail ex: %s", ex.what());
        exit(-1);
    }

    info("parse succ, buf_size: %d, pb: %s", int(buffer1.size()), psc->ShortDebugString().c_str());
}

void simple_codec_bad(){
    Buffer buffer1;
    buffer1.append("44SimpleCodec");
    std::unique_ptr<SimpleCodec> psc;

    try{
        psc.reset(static_cast<SimpleCodec*>(decodePb(buffer1)));
    }catch(const std::exception& ex){
        error("parse pb fail ex: %s", ex.what());
        exit(-1);
    }
}

} // xy

