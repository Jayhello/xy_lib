#pragma once

namespace xy{

class Acceptor;
using AcceptorPtr = Acceptor*;

class NetThread;

class Connection;
using ConnectionPtr = Connection*;

class Server;
using ServerPtr = Server*;

class Handler;
using HandlerPtr = Handler*;

class SendContext;

class RecvContext;

} // xy
