
#include "net_thread.h"
#include "xy_server.h"
#include "comm/logging.h"
#include "comm/comm.h"
#include "data_context.h"

namespace xy{

NetThread::NetThread(Server* pServer, int threadIdx):_pServer(pServer), _threadIdx(threadIdx), _conList(this){
    _ep.create(1024);
    _noticer.init(&_ep);
}

NetThread::~NetThread(){

}

void NetThread::addConnection(ConnectionPtr pc){
    _ep.add(pc->fd(), 0, EPOLLIN);
    _conList.add(pc, pc->getTimeout() + TNOW);
}

void NetThread::terminate(){
    _bTerminate = true;
    _noticer.notify();
}

void NetThread::sendResp(const std::shared_ptr<SendContext>& ctx){

    _noticer.notify();
}

void NetThread::close(const std::shared_ptr<SendContext>& ctx){

    _noticer.notify();
}

void NetThread::processNet(const epoll_event &ev){
    int fd = ev.data.fd;

    ConnectionPtr ptrCon = getConnectionPtr(fd);
    if(nullptr == ptrCon){
        error("NetThread::processNet connection fd %d not exists", fd);
        return;
    }

    if(Epoller::errorEvent(ev)){
        error("NetThread::processNet connection fd %d error event", fd);
        return;
    }

    if(Epoller::readEvent(ev)){
        ptrCon->recvData();
    }

    if(Epoller::writeEvent(ev)){
        if(ptrCon->getSendBuffer().empty()){
            ptrCon->disableWrite();
        }else{
            ptrCon->sendData();
        }
    }
}

void NetThread::processPipe(){

}

void NetThread::run(){
    while(not _bTerminate){
        int num = _ep.wait(1000);

        if(_bTerminate){
            info("new_thread terminate");
            break;
        }

        for(int i = 0; i < num; ++i){
            const epoll_event &ev = _ep.get(i);

            if(_noticer.fd() == ev.data.fd){
                processPipe();
            }else{
                processNet(ev);
            }
        }
    }
}


} // xy