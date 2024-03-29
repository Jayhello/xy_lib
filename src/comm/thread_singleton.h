#pragma once

#include "noncopyable.h"

namespace xy{

template<typename T>
class ThreadSingleton : public noncopyable{
public:
    using value_type = T;
    using pointer    = T*;

    pointer getInstance(){
        static thread_local T instance;
        return &instance;
    }
};

} // xy
