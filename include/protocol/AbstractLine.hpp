#ifndef RAINBACK_PROTOCOL_ABSTRACTLINE_HEADER
#define RAINBACK_PROTOCOL_ABSTRACTLINE_HEADER

#include <QObject>
#include <QIODevice>
#include <QString>

namespace rainback {
namespace protocol {

class AbstractLine : public QObject
{
    Q_OBJECT

protected:
    QIODevice* _io;

public:
    AbstractLine();
    void listen(QIODevice* const io);
    void close();

    QIODevice* io();

public slots:
    virtual void write(const QString& line)=0;
    virtual void flush()=0;

signals:
    void lineRead(const QString& cmd);
};

} // namespace protocol
} // namespace rainback

#endif // RAINBACK_PROTOCOL_ABSTRACTLINE_HEADER
