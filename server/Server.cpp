#include "Server.hpp"

#include <iostream>
#include <QTextStream>
#include <QTcpSocket>

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

    QTextStream stream(socket);

    stream << "No fucking time" << '\n';
    socket->close();
}

