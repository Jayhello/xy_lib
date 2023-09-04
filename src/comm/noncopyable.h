#pragma once

namespace xy{

class noncopyable {
protected:
    // 子类可以构造和析构
    noncopyable() = default;
    ~noncopyable() = default;

private:
    noncopyable(const noncopyable &) = delete;
    noncopyable& operator=(const noncopyable &) = delete;
};

} // util
