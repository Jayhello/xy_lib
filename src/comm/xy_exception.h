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

// errno
int getErrno();

// errno 转化为字符串
string getErrnoDesc(int err);

} // xy
