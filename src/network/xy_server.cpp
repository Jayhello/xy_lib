#include "xy_server.h"
#include "net_util.h"
#include "comm/logging.h"
#include <errno.h>

namespace xy{

Server::Server():_ep(), _stop(false), _iThreadNum(1){
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
    ConnectionPtr pc = new Connection(cfd, pAc, pThread, sIp, iPort);

    pThread->addConnection(pc);

    return 0;
}

void Server::waitForShutdown(){
    for(auto& ptr : _vAcceptor){
        _ep.add(ptr->fd(), 0, EPOLLIN);
    }

    startThread();

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

    stopThread();
}

void Server::startThread(){
    for(int i = 0; i < _iThreadNum; ++i){
        _vNetThread[i]->start();
    }
}

void Server::stopThread(){
    for(int i = 0; i < _iThreadNum; ++i){
        if(_vNetThread[i]->isActive()){
            _vNetThread[i]->getThreadControl().join();
        }
    }
}

void Server::terminate(){

    for(int i = 0; i < _iThreadNum; ++i){
        _vNetThread[i]->terminate();
    }


}





} // xy

