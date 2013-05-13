#include "proxy/TCPServer.hpp"
#include "proxy/Socket.hpp"
#include <iostream>

#include <QTcpSocket>

namespace rainback {
namespace proxy {

void TCPServer::setServer(QTcpServer* const server)
{
    if (_server) {
        disconnect(_server, SIGNAL(newConnection()), this, SLOT(internalNewConnection()));
    }
    _server = server;
    if (!_server) {
        return;
    }
    connect(_server, SIGNAL(newConnection()), this, SLOT(internalNewConnection()));
}

void TCPServer::nextPendingConnection(LuaStack& stack)
{
    QAbstractSocket* socket = _server->nextPendingConnection();
    stack.clear();
    lua::push(stack, socket, true);
}

void TCPServer::internalNewConnection()
{
    emit newConnection();
}

void TCPServer::close()
{
    if (_server) {
        _server->close();
    }
}

} // namespace proxy
} // namespace rainback
