
#include "net_thread.h"
#include "xy_server.h"

namespace xy{

NetThread::NetThread(Server* pServer, int threadIdx):_pServer(pServer), _threadIdx(threadIdx){
}

NetThread::~NetThread(){

}

void NetThread::addConnection(ConnectionPtr){

}

void NetThread::terminate(){
    _bTerminate = true;
    _noticer.notify();
}

void NetThread::run(){
    while(not _bTerminate){
        int num = _ep.wait(1000);

        if(_bTerminate){
            break;
        }

        for(int i = 0; i < num; ++i){
            const epoll_event &ev = _ep.get(i);


        }
    }
}




} // xy