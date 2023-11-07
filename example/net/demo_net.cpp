
#include "demo_net.h"
#include "comm/logging.h"
#include "comm/comm.h"
#include "comm/xy_thread.h"
#include "network/basic/xy_epoll.h"

using namespace xy;

int main(){
    setloglevel(Logger::LTRACE);

    xy::test_epoll();

    return 0;
}

namespace xy{

class EpollThread : public Thread{
public:
    EpollThread(Epoller* ep):_ep(ep){}

    virtual void run()override{
        info("thread start");

        while(1){
            int ret = _ep->wait(1000 * 5);
            info("wait_ret: %d", ret);
            for(int i = 0; i < ret; ++i){
                epoll_event ev = _ep->get(i);
                if(Epoller::readEvent(ev)){
                    char ch;
                    int tmp = EpollNotice::read(ev.data.fd, ch);
                    info("fd: %d can_read ch: %c, ret: %d", ev.data.fd, ch, tmp);
                }else if(Epoller::writeEvent(ev)){
                    info("fd: %d can_write", ev.data.fd);
                }else{
                    break;
                }
            }
        }
    }
private:
    Epoller* _ep;
};

void test_epoll(){
    Epoller ep;
    ep.create(100);

    EpollNotice notice;
    int ret = notice.init(&ep);
    info("notice init ret: %d, fd: %d", ret, notice.fd());

    EpollThread eth(&ep);
    eth.start();

    sleep(1);

    notice.notify('a');
    info("notice ret: %d", ret);

    sleep(11);
    info("exit...");
}



} // xy
