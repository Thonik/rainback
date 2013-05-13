#ifndef RAINBACK_SERVER_HEADER
#define RAINBACK_SERVER_HEADER

#include <QObject>
#include <QTcpServer>

namespace rainback {

class Server : public QObject
{
    Q_OBJECT

    QTcpServer socketServer;
public:
    Server();

public slots:
    void listen();
    void newConnection();
};

} // namespace rainback

#endif // RAINBACK_SERVER_HEADER

// vim: set ts=4 sw=4 :
