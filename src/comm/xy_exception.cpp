#include "xy_exception.h"
#include <cerrno>
#include <cstring>

namespace xy {

Exception::Exception(const string &buffer)
        : _code(0), _buffer(buffer) {
}


Exception::Exception(const string &buffer, int err) {
    _buffer = buffer + " :" + getErrnoDesc(err);
    _code = err;
}

const char *Exception::what() const throw() {
    return _buffer.c_str();
}

Exception::~Exception() {
}

int getErrno() {
    return errno;
}

string getErrnoDesc(int err) {
    string errMsg;
    errMsg = strerror(err);
    return errMsg;
}

} // xy

