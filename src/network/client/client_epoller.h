#pragma once
#include "comm/xy_thread.h"

namespace xy{

class ClientEpoller : public Thread{
public:


private:
    bool                   _terminate;
};

} // xy
