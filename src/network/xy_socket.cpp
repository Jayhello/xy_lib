#include "xy_socket.h"
#include "net_util.h"
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <assert.h>
#include "comm/logging.h"

namespace xy{
Socket::Socket() : _fd(INVALID_SOCKET), _bOwner(true){
}

Socket::~Socket() {
    if (_bOwner) {
        close();
    }
}

void Socket::init(int fd, bool bOwner, int iDomain) {
    if (_bOwner) {
        close();
    }

    _fd = fd;
    _bOwner = bOwner;
}

void Socket::createSocket(int iSocketType, int iDomain) {
    close();

    _fd = socket(iDomain, iSocketType, 0);

    if (_fd < 0) {
        _fd = INVALID_SOCKET;
        THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::createSocket] create socket error");
        // throw SocketException("[Socket::createSocket] create socket error! :" + string(strerror(errno)));
    } else {
        //            ignoreSigPipe();
    }

}

void Socket::bind(const std::string &host, short port){
    _addr = Ip4Addr(host, port);
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int r = setReuseAddr(fd);
    fatalif(r, "set socket reuse option failed");
    r = setReusePort(fd);
    fatalif(r, "set socket reuse port option failed");
    r = addFdFlag(fd, FD_CLOEXEC);
    fatalif(r, "addFdFlag FD_CLOEXEC failed");
    r = ::bind(fd, (struct sockaddr *) &_addr.getAddr(), sizeof(struct sockaddr));

    if (r) {
        close();
        error("bind to %s failed %d %s", _addr.toString().c_str(), errno, strerror(errno));
//        return errno;
    }
}

void Socket::listen(int connBackLog){
    if (::listen(_fd, connBackLog) < 0) {
        THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::listen] listen error");
    }
}

void Socket::close() {
    if (_fd != INVALID_SOCKET) {
        ::close(_fd);
        _fd = INVALID_SOCKET;
    }
}

void Socket::setblock(bool bBlock) {
    assert(_fd > 0);
    setNonBlock(_fd, not bBlock);
}

//int Socket::setSockOpt(int opt, const void *pvOptVal, SOCKET_LEN_TYPE optLen, int level) {
//    return setsockopt(_fd, level, opt, (const char *) pvOptVal, optLen);
//}
//
//int Socket::getSockOpt(int opt, void *pvOptVal, SOCKET_LEN_TYPE &optLen, int level) const {
//    return getsockopt(_fd, level, opt, (char *) pvOptVal, &optLen);
//}

void Socket::setNoCloseWait() {
    linger stLinger;
    stLinger.l_onoff = 1;  //在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
    stLinger.l_linger = 0; //容许逗留的时间为0秒

    if (setSocketOpt(_fd, SO_LINGER, (const void *) &stLinger, sizeof(linger), SOL_SOCKET) == -1) {
        THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setNoCloseWait] error");
    }
}

void Socket::setCloseWait(int delay) {
    linger stLinger;
    stLinger.l_onoff = 1;  //在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
    stLinger.l_linger = delay; //容许逗留的时间为delay秒

    if (setSocketOpt(_fd, SO_LINGER, (const void *) &stLinger, sizeof(linger), SOL_SOCKET) == -1) {
        THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setCloseWait] error");

        // throw SocketException("[Socket::setCloseWait] error", getErrno());
    }
}

void Socket::setCloseWaitDefault() {
    linger stLinger;
    stLinger.l_onoff = 0;
    stLinger.l_linger = 0;

    if (setSocketOpt(_fd, SO_LINGER, (const void *) &stLinger, sizeof(linger), SOL_SOCKET) == -1) {
        THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setCloseWaitDefault] error");
    }
}

void Socket::setTcpNoDelay() {
    int flag = 1;

    if (setSocketOpt(_fd, TCP_NODELAY, (char *) &flag, int(sizeof(int)), IPPROTO_TCP) == -1) {
        THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setTcpNoDelay] error");
    }
}

void Socket::setKeepAlive() {
    int flag = 1;
    if (setSocketOpt(_fd, SO_KEEPALIVE, (char *) &flag, int(sizeof(int)), SOL_SOCKET) == -1) {
        THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setKeepAlive] error");
    }
}

bool Socket::isPending() {
    return getErrno() == EAGAIN;
}

bool Socket::isInProgress() {
    return getErrno() == EINPROGRESS;
}

} // xy
