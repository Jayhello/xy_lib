//
// Created by wenwen on 2023/3/14.
//
#pragma once
#include <exception>
#include <string>

namespace xy{

using namespace std;

class Exception : public std::exception{
public:
    explicit Exception(const string &msg);

    Exception(const string &buffer, int err);

    virtual ~Exception();

    virtual const char* what() const throw();

    int getErrCode() { return _code; }
private:
    int     _code;
    string  _buffer;
};

inline int getErrno() {
    return errno;
}

// errno 转化为字符串
string getErrnoDesc(int err);

} // xy

#define THROW_EXCEPTION_SYSCODE(EX_CLASS, buffer) \
{                               \
int ret = getErrno();           \
throw EX_CLASS(buffer, ret);    \
}
