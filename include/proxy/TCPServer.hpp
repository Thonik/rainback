#ifndef RAINBACK_TCPSERVER_HEADER
#define RAINBACK_TCPSERVER_HEADER

#include <QTcpServer>

namespace rainback {
namespace proxy {

class TCPServer : public QObject
{
    Q_OBJECT

    QTcpServer* _server;
private slots:
    void newConnection();

public:
    TCPServer() :
        _server(nullptr)
    {
    }

    void setServer(QTcpServer* const server);

public slots:
    void close();

signals:
    void newConnection(QTcpSocket* socket);
};

} // namespace proxy
} // namespace rainback

#endif // RAINBACK_TCPSERVER_HEADER

// vim: set ts=4 sw=4 :
