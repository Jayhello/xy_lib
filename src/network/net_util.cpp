#include "net_util.h"
#include <fcntl.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

namespace xy{

int createTcpSocket(){
    return ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

int doConnect(int fd, const string& sIp, int iPort){
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(sIp.c_str());
    serv_addr.sin_port = htons(iPort);

    return ::connect(fd, (sockaddr*)&serv_addr, sizeof(serv_addr));
}

int doBind(int fd, const string& sIp, int iPort){
    int op = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(sIp.c_str());
    serv_addr.sin_port = htons(iPort);

    return bind(fd, (sockaddr*)&serv_addr, sizeof(serv_addr));
}

int doListen(int fd, int n){
    return ::listen(fd, n);
}

int createTcpServerSocket(const string& sIp, int iPort){
    int socketFd = createTcpSocket();
    if(socketFd < 0) return -1;

    setReuseAddr(socketFd);
    setReusePort(socketFd);

    int ret = doBind(socketFd, sIp, iPort);
    if(ret < 0)return -2;

    ret = doListen(socketFd);
    if(ret < 0) return -3;
    return socketFd;
}

int doAccept(int fd, string& sIp, uint16_t& iPort){
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    int cfd = accept(fd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    if(cfd > 0){
        sIp = inet_ntoa(clnt_addr.sin_addr);
        iPort = clnt_addr.sin_port;
    }

    return cfd;
}

int doWrite(int fd, const string& sData){
    return ::write(fd, sData.c_str(), sData.size());
}

int doWrite(int fd, const char* str, size_t size){
    return ::write(fd, str, size);
}

int doRead(int fd, string& sData, size_t iReadSize){
    // 如下不能直接read(fd, string) 有问题
    //    sData.reserve(iReadSize + 1);
    //    ssize_t read_bytes = read(fd, const_cast<char*>(sData.data()), iReadSize);
    //    sData.resize(read_bytes > 0 ? read_bytes + 1 : 0);
    char arr[1024]= {0};
    ssize_t read_bytes = read(fd, arr, iReadSize);
    if(read_bytes > 0){
        sData.assign(arr, read_bytes);
    }

    return int(read_bytes);
}

// < 0 错误 , > 0 写的数据size  (这个write没有返回0的)
int doSend(int fd, const string& sData){
    return ::send(fd, sData.c_str(), sData.size(), 0);
}

// < 0 错误,  = 0 对方关闭了链接, > 0 写的数据size
int doRecv(int fd, string& sData, size_t iReadSize){
    char arr[1024]= {0};
    ssize_t read_bytes = recv(fd, arr, iReadSize, 0);
    if(read_bytes > 0){
        sData.assign(arr, read_bytes);
    }

    return int(read_bytes);
}

int doClose(int fd){
    return ::close(fd);
}

int setReuseAddr(int fd){
    int flag = 1;
    return setSocketOpt(fd, SO_REUSEADDR, (void*)(&flag), (socklen_t)(sizeof(flag)), SOL_SOCKET);
}

int setReusePort(int fd){
    int flag = 1;
    return setSocketOpt(fd, SO_REUSEPORT, (void*)(&flag), (socklen_t)(sizeof(flag)), SOL_SOCKET);
}

int setSocketOpt(int fd, int opt, const void* val, socklen_t opt_len, int level){
    return setsockopt(fd, level, opt, &val, opt_len);;
}

int setNonBlock(int fd, bool value) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        return errno;
    }
    if (value) {
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
    return fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
}

int setKeepAlive(int fd){
    int flag = 1;
    return setSocketOpt(fd, SO_REUSEPORT, (void*)(&flag), (socklen_t)(sizeof(flag)), SOL_SOCKET);
}

int setTcpNoDelay(int fd){
    int flag = 1;
    return setSocketOpt(fd, TCP_NODELAY, (void*)(&flag), (socklen_t)(sizeof(flag)), IPPROTO_TCP);
}

int setCloseWaitDefault(int fd){
    linger stLinger;
    stLinger.l_onoff = 0;
    stLinger.l_linger = 0;

    return setSocketOpt(fd, SO_LINGER, (void*)(&stLinger), (socklen_t)(sizeof(stLinger)), SOL_SOCKET);
}

void setNoBlock(int fd){
    int flag = fcntl(fd, F_GETFL, 0);

    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
}

int addFdFlag(int fd, int flag) {
    int ret = fcntl(fd, F_GETFD);
    return fcntl(fd, F_SETFD, ret | flag);
}

struct in_addr getHostByName(const std::string &host) {
    struct in_addr addr;
    struct hostent *he = gethostbyname(host.c_str());
    if (he && he->h_addrtype == AF_INET) {
        addr = *reinterpret_cast<struct in_addr *>(he->h_addr);
    } else {
        addr.s_addr = INADDR_NONE;
    }
    return addr;
}

} // xy
