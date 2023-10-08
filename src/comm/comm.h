//
// Created by wenwen on 2023/3/12.
//
#pragma once
#include <stdlib.h>
#include <string.h>
#include <functional>
#include <string>
#include <utility>
#include "noncopyable.h"

#define TNOW     xy::now()
#define TNOWMS   xy::now2ms()

namespace xy{

using namespace std;

// 获取当前时间的的毫秒数.
int64_t now2ms();

// 获取当前时间的的秒数.
int64_t now();

// 取出当前时间的微秒.
int64_t now2us();

int64_t timeMicro();

inline int64_t timeMilli() { return timeMicro() / 1000; }

std::string format(const char *fmt, ...);

std::string readableTime(time_t t);

inline int64_t atoi(const char *b, const char *e) { return strtol(b, (char **) &e, 10); }

inline int64_t atoi(const char *b) { return atoi(b, b + strlen(b)); }

uint64_t gettid();

void sleep(uint32_t sec);

void msleep(uint32_t ms);

int GetRandomRange(int min, int max);

struct ExitCaller : private noncopyable {
    ~ExitCaller() { functor_(); }
    ExitCaller(std::function<void()> &&functor) : functor_(std::move(functor)) {}

private:
    std::function<void()> functor_;
};

} // xy
