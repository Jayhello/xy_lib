
#include "xy_epoll.h"
#include <fcntl.h>
#include <netinet/tcp.h>
#include <assert.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include "net_util.h"
#include "comm/logging.h"

namespace xy{

Epoller::Epoller():_epFd(-1), _pEvs(nullptr), _size(100){}

Epoller::~Epoller(){
    if(nullptr != _pEvs){
        delete [] _pEvs;
    }

    close();
}

void Epoller::create(int iSize){
    _epFd = epoll_create(iSize);

    _size = iSize;
    _pEvs = new epoll_event[_size];
    bzero(_pEvs, sizeof(*_pEvs) * _size);
}

void Epoller::close(){
    if(_epFd > 0){
        ::close(_epFd);
        _epFd = -1;
    }
}

// 添加监听句柄(iValue透传存储于 epoll_event中, event: EPOLLIN|EPOLLOUT)
int Epoller::add(int fd, uint64_t iValue, int32_t event){
    return ctrl(fd, iValue, event, EPOLL_CTL_ADD);
}

// 修改句柄事件
int Epoller::mod(int fd, uint64_t iValue, int32_t event){
    return ctrl(fd, iValue, event, EPOLL_CTL_MOD);
}

int Epoller::del(int fd, uint64_t iValue, int32_t event){
    return ctrl(fd, iValue, event, EPOLL_CTL_DEL);
}

int Epoller::ctrl(int fd, uint64_t iValue, uint32_t events, int op){
    struct epoll_event ev;
    ev.events = events;
    ev.data.u64 = iValue;
    ev.data.fd = fd;

    return epoll_ctl(_epFd, op, fd, &ev);
}

// 等待一次, 返回触发的事件数
int Epoller::wait(int millsecond){
    return epoll_wait(_epFd, _pEvs, _size, millsecond);
}

// wait 之后可以用此接口获取触发的事件
epoll_event & Epoller::get(int i){
    assert(_pEvs != 0);
    return _pEvs[i];
}

// 是否有读事件
bool Epoller::readEvent(const epoll_event &ev){
    if (ev.events & EPOLLIN) {
        return true;
    }

    return false;
}

// 是否有写事件
bool Epoller::writeEvent(const epoll_event &ev){
    if (ev.events & EPOLLOUT) {
        return true;
    }

    return false;
}

// 是否有异常事件
bool Epoller::errorEvent(const epoll_event &ev){
    if (ev.events & EPOLLERR || ev.events & EPOLLHUP) {
        return true;
    }

    return false;
}

// ------------------------------------------------------------------
EpollNotice::EpollNotice():_fd(-1), _pEp(nullptr){
    _wakeupFds[0] = -1;
    _wakeupFds[1] = -1;
}

EpollNotice::~EpollNotice(){
    release();
}

int EpollNotice::init(Epoller *pEp){
    _pEp = pEp;

    int r = pipe(_wakeupFds);
    fatalif(r, "pipe failed %d %s", errno, strerror(errno));
    r = addFdFlag(_wakeupFds[0], FD_CLOEXEC);
    fatalif(r, "addFdFlag failed %d %s", errno, strerror(errno));
    r = addFdFlag(_wakeupFds[1], FD_CLOEXEC);
    fatalif(r, "addFdFlag failed %d %s", errno, strerror(errno));
    trace("wakeup pipe created %d %d", _wakeupFds[0], _wakeupFds[1]);
    return pEp->add(_wakeupFds[0], _wakeupFds[0], EPOLLIN);

//    _fd = createTcpSocket();
//    _fd = socket(AF_INET, SOCK_DGRAM, 0);   // 这里得创建的是 报文的 udp
//
//    if(_fd < 0) return -1;
//
//    _pEp = pEp;
//    return pEp->add(_fd, _fd, EPOLLIN | EPOLLOUT);   // 如果写事件的话, 那么会一直有
//    return pEp->add(_fd, _fd, EPOLLIN);
}

int EpollNotice::notify(char c){
//    return _pEp->mod(_fd, _fd, EPOLLIN | EPOLLOUT);  // 这个不行
//    return _pEp->mod(_fd, _fd, EPOLLIN);
//    return ::write(_fd, &c, 1);
    int r = write(_wakeupFds[1], &c, 1);
    fatalif(r <= 0, "write error wd %d %d %s", r, errno, strerror(errno));
    return r;
}

int EpollNotice::read(int fd, char& c){
    return ::read(fd, &c, sizeof c);
}

int EpollNotice::release(){
    if(_wakeupFds[0] > 0){
        _pEp->del(_wakeupFds[0], 0, EPOLLIN);
        ::close(_wakeupFds[0]);
        _wakeupFds[0] = -1;
    }

    if(_wakeupFds[1] > 0){
        ::close(_wakeupFds[1]);
        _wakeupFds[1] = -1;
    }

    return 0;
}

int EpollNotice::fd()const{
    return _wakeupFds[0];
}

} // xy
