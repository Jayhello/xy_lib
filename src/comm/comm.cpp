//
// Created by wenwen on 2023/3/12.
//

#include "comm.h"
#include <stdarg.h>
#include <algorithm>
#include <chrono>
#include <memory>
#include <pthread.h>
#include <random>

namespace xy{

int64_t timeMicro() {
    std::chrono::time_point<std::chrono::system_clock> p = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(p.time_since_epoch()).count();
}

} // xy