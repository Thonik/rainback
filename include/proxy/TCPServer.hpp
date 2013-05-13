#ifndef RAINBACK_PROXY_TCPSERVER_HEADER
#define RAINBACK_PROXY_TCPSERVER_HEADER

#include <QTcpServer>
#include <lua-cxx/LuaStack.hpp>

namespace rainback {
namespace proxy {

class TCPServer : public QObject
{
    Q_OBJECT

    QTcpServer* _server;

private slots:
    void internalNewConnection();

public:
    TCPServer() :
        _server(nullptr)
    {
    }

    void setServer(QTcpServer* const server);

    Q_INVOKABLE void nextPendingConnection(LuaStack& stack);
public slots:
    void close();

signals:
    void newConnection();
};

} // namespace proxy
} // namespace rainback

#endif // RAINBACK_PROXY_TCPSERVER_HEADER

// vim: set ts=4 sw=4 :
