
#include "demo_client.h"
#include "comm/logging.h"
#include "comm/comm.h"
#include "network/basic/xy_socket.h"
#include "network/basic/net_util.h"
#include "network/codec/xy_codec.h"

using namespace xy;

int main(){
    setloglevel(Logger::LTRACE);

//    echo_client();
    subpackage_client();

    return 0;
}

namespace xy{

void echo_client(){
    ScopeLog Log;

    Socket client;
    int ret = client.connect("127.0.0.1", 8088);
    return_if(ret < 0, "connect fail");

    Log << "connect succ";
    info("connect succ, fd: %d", client.getfd());

    LengthCodec codec;
    Buffer buf;
    codec.encode("abc", buf);
    info("buf size: %d, %s", int(buf.toString().size()), buf.toString().c_str());

    ret = doWrite(client.getfd(), buf.toString());
    return_if(ret < 0, "write fail");
    Log << "write_ret" << ret;

    string sData;
    ret = doRead(client.getfd(), sData, 1024);
    return_if(ret < 0, "read fail");
    Log << "read_ret" << ret;

    Log << "succ";

}

void subpackage_client(){
    ScopeLog Log;

    Socket client;
    int ret = client.connect("127.0.0.1", 8088);
    return_if(ret < 0, "connect fail");

    Log << "connect succ";
    info("connect succ, fd: %d", client.getfd());

    LengthCodec codec;
    Buffer buf;
    codec.encode("abc", buf);
    info("buf size: %d, %s", int(buf.toString().size()), buf.toString().c_str());

    size_t iWriteSize = 3;
    ret = doWrite(client.getfd(), buf.data(), iWriteSize);
    return_if(ret < 0, "write fail");
    Log << "write_ret" << ret;

    sleep(5);   // 等待 5 s

    buf.consume(iWriteSize);
    ret = doWrite(client.getfd(), buf.data(), buf.size());
    return_if(ret < 0, "write fail");
    Log << "write_left_ret" << ret;

    string sData;
    ret = doRead(client.getfd(), sData, 1024);
    return_if(ret < 0, "read fail");
    Log << "read_size" << ret << ", str:" << sData;

    Log << "succ";
}

} // xy

