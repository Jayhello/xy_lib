#pragma once
#include <string>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "src/network/basic/xy_ipaddr.h"
#include "comm/xy_exception.h"

#define INVALID_SOCKET -1

namespace xy{

struct SocketException : public Exception {
    SocketException(const string &buffer) : Exception(buffer) {};
    SocketException(const string &buffer, int err) : Exception(buffer, err) {};
    ~SocketException() throw() {};
};    
    
class Socket{
public:
    Socket();

    virtual ~Socket();

    void init(int fd, bool bOwner, int iDomain = AF_INET);

    // 生成socket, 如果已经存在以前的socket, 则释放掉, 生成新的.
    void createSocket(int iSocketType = SOCK_STREAM, int iDomain = AF_INET);

    int getfd() const { return _fd; }

    bool isValid() const { return _fd > 0;}

    void close();

    // host填空则是 0.0.0.0
    void bind(const std::string &host, short port);

    void listen(int connBackLog);

    // (简单的同步链接)host填空则是 0.0.0.0
    int connect(const std::string &host, short port);

    void setblock(bool bBlock = false);

    void setNoCloseWait();

    void setCloseWait(int delay = 30);

    void setCloseWaitDefault();

    void setTcpNoDelay();

    void setKeepAlive();

    // 判断当前socket是否处于EAGAIN/WSAEWOULDBLOCK(异步send/recv函数返回值时判断)
    static bool isPending();

    // 判断当前socket是否处于EINPROGRESS/WSAEWOULDBLOC(异步connect返回值时判断)
    static bool isInProgress();

private:
    int      _fd;
    bool     _bOwner;
    Ip4Addr  _addr;
};

} // xy
