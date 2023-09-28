#include "xy_socket.h"
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

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
        _iDomain = iDomain;
    }

    void Socket::createSocket(int iSocketType, int iDomain) {
        close();

        _iDomain = iDomain;
        _fd = socket(iDomain, iSocketType, 0);

        if (_fd < 0) {
            _fd = INVALID_SOCKET;
            THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::createSocket] create socket error");
            // throw SocketException("[Socket::createSocket] create socket error! :" + string(strerror(errno)));
        } else {
            //            ignoreSigPipe();
        }

    }

    void bind(const std::string &host, short port){
        addr_ = Ip4Addr(host, port);
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        int r = setReuseAddr(fd);
        fatalif(r, "set socket reuse option failed");
        r = setReusePort(fd, reusePort);
        fatalif(r, "set socket reuse port option failed");
        r = addFdFlag(fd, FD_CLOEXEC);
        fatalif(r, "addFdFlag FD_CLOEXEC failed");
        r = ::bind(fd, (struct sockaddr *) &addr_.getAddr(), sizeof(struct sockaddr));

        if (r) {
            close(fd);
            error("bind to %s failed %d %s", addr_.toString().c_str(), errno, strerror(errno));
            return errno;
        }
    }

    void listen(int connBackLog){

    }

    void Socket::close() {
        if (_fd != INVALID_SOCKET) {
            ::close(_fd);
            _fd = INVALID_SOCKET;
        }
    }

    void Socket::setblock(bool bBlock) {
        assert(_fd != INVALID_SOCKET);

        setblock(_fd, bBlock);
    }

    int Socket::setSockOpt(int opt, const void *pvOptVal, SOCKET_LEN_TYPE optLen, int level) {
        return setsockopt(_fd, level, opt, (const char *) pvOptVal, optLen);
    }

    int Socket::getSockOpt(int opt, void *pvOptVal, SOCKET_LEN_TYPE &optLen, int level) const {
        return getsockopt(_fd, level, opt, (char *) pvOptVal, &optLen);
    }

    void Socket::setNoCloseWait() {
        linger stLinger;
        stLinger.l_onoff = 1;  //在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
        stLinger.l_linger = 0; //容许逗留的时间为0秒

        if (setSockOpt(SO_LINGER, (const void *) &stLinger, sizeof(linger), SOL_SOCKET) == -1) {
            THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setNoCloseWait] error");
        }
    }

    void Socket::setCloseWait(int delay) {
        linger stLinger;
        stLinger.l_onoff = 1;  //在close socket调用后, 但是还有数据没发送完毕的时候容许逗留
        stLinger.l_linger = delay; //容许逗留的时间为delay秒

        if (setSockOpt(SO_LINGER, (const void *) &stLinger, sizeof(linger), SOL_SOCKET) == -1) {
            THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setCloseWait] error");

            // throw SocketException("[Socket::setCloseWait] error", getErrno());
        }
    }

    void Socket::setCloseWaitDefault() {
        linger stLinger;
        stLinger.l_onoff = 0;
        stLinger.l_linger = 0;

        if (setSockOpt(SO_LINGER, (const void *) &stLinger, sizeof(linger), SOL_SOCKET) == -1) {
            THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setCloseWaitDefault] error");
        }
    }

    void Socket::setTcpNoDelay() {
        int flag = 1;

        if (setSockOpt(TCP_NODELAY, (char *) &flag, int(sizeof(int)), IPPROTO_TCP) == -1) {
            THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setTcpNoDelay] error");
        }
    }

    void Socket::setKeepAlive() {
        int flag = 1;
        if (setSockOpt(SO_KEEPALIVE, (char *) &flag, int(sizeof(int)), SOL_SOCKET) == -1) {
            THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setKeepAlive] error");
        }
    }

    void Socket::setblock(SOCKET_TYPE fd, bool bBlock) {
        int val = 0;

        if ((val = fcntl(fd, F_GETFL, 0)) == -1) {
            THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setblock] fcntl [F_GETFL] error");
        }

        if (!bBlock) {
            val |= O_NONBLOCK;
        } else {
            val &= ~O_NONBLOCK;
        }

        if (fcntl(fd, F_SETFL, val) == -1) {
            THROW_EXCEPTION_SYSCODE(SocketException, "[Socket::setblock] fcntl [F_SETFL] error");
        }
    }

    bool Socket::isPending() {
        return getErrno() == EAGAIN;
    }

    bool Socket::isInProgress() {
        return getErrno() == EINPROGRESS;
    }

} // xy
