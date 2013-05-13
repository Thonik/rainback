#include "proxy/TCPServer.hpp"
#include <iostream>

#include <QTcpSocket>

namespace rainback {
namespace proxy {

void TCPServer::setServer(QTcpServer* const server)
{
    if (_server) {
        disconnect(_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    }
    _server = server;
    if (!_server) {
        return;
    }
    connect(_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void TCPServer::newConnection()
{
    QTcpSocket* const socket = _server->nextPendingConnection();
    std::cout << "New connection from " << socket->peerAddress().toString().toStdString() << std::endl;

    // It is assumed whoever is listening to this signal will manage the socket's memory.
    emit newConnection(socket);
}

void TCPServer::close()
{
    if (_server) {
        _server->close();
    }
}

} // namespace proxy
} // namespace rainback
