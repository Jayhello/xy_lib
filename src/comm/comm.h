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

namespace xy{

using namespace std;

int64_t timeMicro();

int64_t timeMilli() { return timeMicro() / 1000; }

std::string format(const char *fmt, ...);

std::string readableTime(time_t t);

int64_t atoi(const char *b, const char *e) { return strtol(b, (char **) &e, 10); }

int64_t atoi(const char *b) { return atoi(b, b + strlen(b)); }

uint64_t gettid();

int GetRandomRange(int min, int max);

struct ExitCaller : private noncopyable {
    ~ExitCaller() { functor_(); }
    ExitCaller(std::function<void()> &&functor) : functor_(std::move(functor)) {}

private:
    std::function<void()> functor_;
};

} // xy
