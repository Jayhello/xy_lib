#pragma once
#include <string>
#include <vector>

namespace xy{

struct RecvContext{
    RecvContext(int fd, const std::string& ip, uint16_t port):_fd(fd), _ip(ip), _port(port){}

    int fd()const{return _fd;}

    const std::string& ip()const{return _ip;}

    uint16_t port()const{return _port;}

    std::vector<char>& buffer(){return _buf;}

    const std::vector<char>& buffer()const{return _buf;}

    int                 _fd;
    std::string         _ip;
    uint16_t            _port;
    std::vector<char>   _buf;
};

struct SendContext{
    SendContext(int fd, const std::string& ip, uint16_t port, char cmd):_fd(fd), _ip(ip), _port(port), _cmd(cmd){}

    int fd()const{return _fd;}

    const std::string& ip()const{return _ip;}

    uint16_t port()const{return _port;}

    char cmd()const{
        return _cmd;
    }

    std::vector<char>& buffer(){return _buf;}

    const std::vector<char>& buffer()const{return _buf;}

    int                 _fd;
    std::string         _ip;
    uint16_t            _port;
    char                _cmd;
    std::vector<char>   _buf;
};

} // xy
