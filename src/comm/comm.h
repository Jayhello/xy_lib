//
// Created by wenwen on 2023/3/12.
//
#pragma once
#include <stdlib.h>
#include <string.h>
#include <functional>
#include <string>
#include <utility>

namespace xy{

int64_t timeMicro();
int64_t timeMilli() { return timeMicro() / 1000; }

} // xy
