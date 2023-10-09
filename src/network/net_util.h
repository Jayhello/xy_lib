
#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

namespace xy{

using std::string;

// < 0 错误
int createTcpSocket();

// < 0 错误
int doConnect(int fd, const string& sIp, int iPort);

int doBind(int fd, const string& sIp, int iPort);

int doListen(int fd, int n = SOMAXCONN);

int doAccept(int fd, string& sIp, uint16_t& iPort);

// create, bind, listen, ret > 0 succ
int createTcpServerSocket(const string& sIp, int iPort);

// < 0 错误 , > 0 写的数据size  (这个write没有返回0的)
// todo read 这里的参数 sData 是否可以优化下, const char* str, len
int doWrite(int fd, const string& sData);

// < 0 错误,  = 0 对方关闭了链接, > 0 写的数据size
int doRead(int fd, string& sData, size_t iReadSize);

// < 0 错误 , > 0 写的数据size  (这个write没有返回0的)
int doSend(int fd, const string& sData);

// < 0 错误,  = 0 对方关闭了链接, > 0 写的数据size
int doRecv(int fd, string& sData, size_t iReadSize);

int doClose(int fd);

int setNonBlock(int fd, bool value = true);

int setReuseAddr(int fd);

int setReusePort(int fd);

int setSocketOpt(int fd, int opt, const void* val, socklen_t opt_len, int level);

int addFdFlag(int fd, int flag);

struct in_addr getHostByName(const std::string &host);

static const int kLittleEndian = LITTLE_ENDIAN;

inline uint16_t htobe(uint16_t v) {
    if (!kLittleEndian) {
        return v;
    }
    unsigned char *pv = (unsigned char *) &v;
    return uint16_t(pv[0]) << 8 | uint16_t(pv[1]);
}
inline uint32_t htobe(uint32_t v) {
    if (!kLittleEndian) {
        return v;
    }
    unsigned char *pv = (unsigned char *) &v;
    return uint32_t(pv[0]) << 24 | uint32_t(pv[1]) << 16 | uint32_t(pv[2]) << 8 | uint32_t(pv[3]);
}
inline uint64_t htobe(uint64_t v) {
    if (!kLittleEndian) {
        return v;
    }
    unsigned char *pv = (unsigned char *) &v;
    return uint64_t(pv[0]) << 56 | uint64_t(pv[1]) << 48 | uint64_t(pv[2]) << 40 | uint64_t(pv[3]) << 32 | uint64_t(pv[4]) << 24 | uint64_t(pv[5]) << 16 |
    uint64_t(pv[6]) << 8 | uint64_t(pv[7]);
}
inline int16_t htobe(int16_t v) {
    return (int16_t) htobe((uint16_t) v);
}
inline int32_t htobe(int32_t v) {
    return (int32_t) htobe((uint32_t) v);
}
inline int64_t htobe(int64_t v) {
    return (int64_t) htobe((uint64_t) v);
}

template <class T>
T hton(T v) {
    return htobe(v);
}

template <class T>
T ntoh(T v) {
    return htobe(v);
}

} // xy
