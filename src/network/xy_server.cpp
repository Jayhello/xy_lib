#include "xy_server.h"
#include "net_util.h"
#include "data_context.h"
#include "comm/logging.h"
#include <errno.h>

namespace xy{

Server::Server(int iThreadNum):_ep(), _stop(false), _iThreadNum(iThreadNum){
}

Server::~Server(){
}

int Server::init(){
    _ep.create(1024);

    for(int i = 0; i < _iThreadNum; ++i){
        NetThread * pth = new NetThread(this, i);
        _vNetThread.push_back(pth);
    }

    return 0;
}

void Server::addAcceptor(AcceptorPtr ptr){
    _vAcceptor.push_back(ptr);
    _mFdAcceptor[ptr->fd()] = ptr;
}

int Server::accept(int fd){
    string sIp;
    uint16_t iPort;
    int cfd = doAccept(fd, sIp, iPort);
    if(cfd < 0){
        error("fd: %d accept error ret: %d, error: %d, %s", fd, cfd, errno, strerror(errno));
        return -1;
    }

    debug("fd: %d accept fd: %d, %s:%d", fd, cfd, sIp.c_str(), iPort);

//    if(max_connection){
//        close()
//    }
    auto pAc = _mFdAcceptor[fd];
    auto pThread = getNetThread(cfd);
    ConnectionPtr pc = new Connection(cfd, pAc, pThread, sIp, iPort, pAc->getTimeout());

    pThread->addConnection(pc);

    return 0;
}

void Server::waitForShutdown(){
    for(auto& ptr : _vAcceptor){
        _ep.add(ptr->fd(), 0, EPOLLIN);
    }

    startNetThread();

    startAcceptorHandles();

    while(not _stop){
        int num = _ep.wait(300);

        for(int i = 0; i < num; ++i){
            const epoll_event &ev = _ep.get(i);

            try{
                accept(ev.data.fd);
            } catch (const std::exception& ex) {

            }
        }
    }

    stopNetThread();

    stopAcceptorHandles();
}

void Server::startNetThread(){
    for(int i = 0; i < _iThreadNum; ++i){
        _vNetThread[i]->start();
    }
}

void Server::stopNetThread(){
    for(int i = 0; i < _iThreadNum; ++i){
        if(_vNetThread[i]->isActive()){
            _vNetThread[i]->getThreadControl().join();
        }
    }
}

void Server::startAcceptorHandles(){
    for(auto pa : _vAcceptor){
        for(auto ph : pa->getHandles()){
            ph->start();
        }
    }
}

void Server::stopAcceptorHandles(){
    for(auto pa : _vAcceptor){
        for(auto ph : pa->getHandles()){
            if(ph->isActive()){
                ph->getThreadControl().join();
            }
        }
    }
}

void Server::terminate(){
    _stop = true;

    for(int i = 0; i < _iThreadNum; ++i){
        _vNetThread[i]->terminate();
    }

    // handle 那里会判断stop状态
}

void Server::sendResp(const std::shared_ptr<SendContext>& ctx){
    auto pThread = getNetThread(ctx->fd());
    pThread->sendResp(ctx);
}

void Server::close(const std::shared_ptr<SendContext>& ctx){
    auto pThread = getNetThread(ctx->fd());

}



} // xy

