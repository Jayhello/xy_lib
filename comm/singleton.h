//
// Created by wenwen on 2023/3/12.
//

#pragma once
#include "noncopyable.h"

namespace xy{

template<typename T>
class Singleton : public noncopyable{
public:
    using value_type = T;
    using pointer    = T*;

    pointer getInstance(){
        static T instance;
        return &instance;
    }
};

} // xy
