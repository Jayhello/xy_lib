
#include <iostream>
#include "demo_comm.h"
#include "comm/logging.h"
#include "comm/comm.h"
#include "comm/xy_thread.h"
#include "comm/slice.h"
#include "comm/block_queue.h"
#include "common_pb/common.pb.h"

int main(int argc, char** argv){

    std::cout << __FUNCTION__ << " do" << std::endl;

//    xy::simple_log();
//    xy::simple_thread();
//    xy::simple_slice();
//    xy::simple_block_que();
    xy::simple_pb();

    return 0;
}

namespace xy{

void simple_log(){
    info("this is simple");
}

class TestThread : public Thread{
public:
    virtual void run()override{
        info("this thread will sleep sec");
        sleep(3);
    }
};

void simple_thread(){
    try{
        TestThread th;
        th.start();
        th.start();
        sleep(3);
    } catch (const std::exception& ex) {
        std::cout << "ex: " << ex.what() << endl;
    }

}

void simple_slice(){
    Slice slice("abc");
    std::cout << slice.toString() << std::endl;

    {
        std::vector<char> vec(slice.toVecChar());
        std::cout << "vec size: " << vec.size() << ",";
        for(auto c : vec) std::cout << c;
        std::cout << std::endl;
    }

}

void simple_block_que(){
    BlockQueue<int> blockQueue;

    std::thread produce([&blockQueue]{
        for(int i = 0; i < 100; ++i){
            blockQueue.push_back(i);
            info("produce que: %d", i);
            sleep(1);
        }
    });

    std::thread consume([&blockQueue]{
        for(int i = 0; i < 100; ++i){
            int a = 0;
            bool res = blockQueue.pop_wait(&a, 500);
            info("consume que res: %d, v: %d", res, a);
        }
    });

    produce.detach();
    consume.detach();

    sleep(10);
    info("exit produce_consume");

    blockQueue.setStop();
}

void simple_pb(){
    common::Info info;
    info.set_id(1);
    info.set_name("xy");
    info.PrintDebugString();
}

} // xy

