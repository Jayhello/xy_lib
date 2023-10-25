//
// Created by Administrator on 2023/9/26.
//
#pragma once
#include <sys/epoll.h>
#include <string>

namespace xy{

class Epoller{
public:
    Epoller(const std::string& name = "");

    ~Epoller();

    void create(int iSize);

    void close();

    // 添加监听句柄(iValue透传存储于 epoll_event中, event: EPOLLIN|EPOLLOUT)
    int add(int fd, uint64_t iValue, int32_t event);

    // 修改句柄事件
    int mod(int fd, uint64_t iValue, int32_t event);

    int del(int fd, uint64_t data, int32_t event);

    // 等待一次, 返回触发的事件数
    int wait(int millsecond);

    // wait 之后可以用此接口获取触发的事件
    epoll_event &get(int i);// { assert(_pevs != 0); return _pevs[i]; }

    std::string toString()const;

    // 是否有读事件
    static bool readEvent(const epoll_event &ev);

    // 是否有写事件
    static bool writeEvent(const epoll_event &ev);

    // 是否有异常事件
    static bool errorEvent(const epoll_event &ev);

protected:
    int ctrl(int fd, uint64_t iValue, uint32_t events, int op);

private:
    std::string _sName;
    int         _epFd;
    epoll_event *_pEvs;   // 事件集合
    int         _size;
};


class EpollNotice{
public:
    EpollNotice();

    ~EpollNotice();

    int init(Epoller *pEp);

    int notify(char c = 'c');

    int readNotify(char& c);

    // read notify写入的字符
    static int read(int fd, char& c);

    int release();

    int fd()const;

private:
    int        _fd;
    Epoller    *_pEp;
    int        _wakeupFds[2];    // 这里用 pipe管道做通知
};

} // xy
