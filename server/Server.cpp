#include "Server.hpp"

#include <iostream>
#include <QTextStream>
#include <QTcpSocket>

#include "protocol/Human.hpp"

namespace rainback {

Server::Server()
{
    connect(&socketServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void Server::listen()
{
    socketServer.listen(QHostAddress::Any, 28123);
}

void Server::newConnection()
{
    QTcpSocket* const socket = socketServer.nextPendingConnection();
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));

    auto ptl = new protocol::Human;
    std::cout << "New connection from " << socket->peerAddress().toString().toStdString() << std::endl;
    ptl->listen(socket);
}

} // namespace rainback
