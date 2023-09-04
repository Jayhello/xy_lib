
#include <iostream>
#include "demo_comm.h"
#include "comm/logging.h"

int main(int argc, char** argv){

    std::cout << __FUNCTION__ << " do" << std::endl;

    xy::simple_log();

    return 0;
}

namespace xy{

void simple_log(){
    info("this is simple");
}

} // xy

