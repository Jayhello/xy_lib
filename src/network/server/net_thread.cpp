
#include "net_thread.h"
#include "xy_server.h"
#include "comm/logging.h"
#include "comm/comm.h"
#include "network/server/data_context.h"
#include "network/basic/net_util.h"

namespace xy{

NetThread::NetThread(Server* pServer, int threadIdx):_pServer(pServer), _threadIdx(threadIdx),_ep("NetThead"),_conManager(this){
    _ep.create(1024);
    _noticer.init(&_ep);
}

NetThread::~NetThread(){

}

void NetThread::addConnection(ConnectionPtr pc){
    _ep.add(pc->fd(), 0, EPOLLIN);
    _conManager.add(pc, pc->getTimeout() + TNOW);
}

void NetThread::terminate(){
    _bTerminate = true;
    _noticer.notify();
}

void NetThread::sendResp(const std::shared_ptr<SendContext>& ctx){
    _sendQueue.push_back(ctx);
    _noticer.notify();
}

void NetThread::close(const std::shared_ptr<SendContext>& ctx){

    _noticer.notify();
}

void NetThread::delConnection(ConnectionPtr pCon){
    _ep.del(pCon->fd(), 0, 0);
    doClose(pCon->fd());
    _conManager.del(pCon);
}


void NetThread::processPipe(){
    char ch;
    _noticer.readNotify(ch);
    trace("[processPipe] read: %c", ch);

    while(not _sendQueue.empty()){
        std::shared_ptr<SendContext> ctx;
        bool res = _sendQueue.pop_wait(&ctx, 100);   // 到了这里基本都是有

        if(not res or (!ctx.get())){
            continue;
        }

        ConnectionPtr pConn = getConnectionPtr(ctx->fd());
        if(nullptr == pConn){
            continue;
        }

        switch(ctx->cmd()){
            case 's':{
                int ret = pConn->sendData(ctx->buffer());
                if(ret < 0){

                }
                break;
            }case 'c':{

                break;
            }default:{

            }
        }
    }
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
        int tmp = ptrCon->recvData();
//        trace("fd: %d, read ret: %d", ptrCon->fd(), tmp);
        if(tmp < 0){
            delConnection(ptrCon);
            return;
        }
    }

    if(Epoller::writeEvent(ev)){
        trace("fd: %d, write event", ptrCon->fd());
        if(ptrCon->getSendBuffer().empty()){
            ptrCon->disableWrite();
        }else{
            ptrCon->sendData();
        }
    }
}

void NetThread::run(){
    info("[NetThread] start run");

    while(not _bTerminate){
        int num = _ep.wait(3000);
        trace("[NetThread] wait, ret: %d", num);

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