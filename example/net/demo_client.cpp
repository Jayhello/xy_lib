
#include "demo_client.h"
#include "comm/logging.h"
#include "comm/comm.h"

using namespace xy;

int main(){
    setloglevel(Logger::LTRACE);

    echo_client();

    return 0;
}

namespace xy{

void echo_client(){

}

} // xy

