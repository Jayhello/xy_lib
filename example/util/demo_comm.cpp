
#include <iostream>
#include "demo_comm.h"
#include "comm/logging.h"
#include "comm/comm.h"
#include "comm/xy_thread.h"
#include "comm/slice.h"

int main(int argc, char** argv){

    std::cout << __FUNCTION__ << " do" << std::endl;

//    xy::simple_log();
//    xy::simple_thread();
    xy::simple_slice();

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

} // xy

