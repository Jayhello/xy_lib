#include "connection.h"

namespace xy{

Connection::Connection(int fd, AcceptorPtr pAcceptor, NetThread* pTh, const string& ip, uint16_t port):
    _fd(fd), _pAcceptor(pAcceptor), _pTh(pTh){
}

Connection::~Connection(){

}

} // xy
