#ifndef RAINBACK_PASCAL_HEADER
#define RAINBACK_PASCAL_HEADER

#include <QObject>
#include <QIODevice>

namespace rainback {
namespace protocol {

class Pascal : public QObject
{
    Q_OBJECT

    typedef quint32 packetsize;
    QIODevice* _io;

    packetsize lineSize;

public:
    Pascal();
    void listen(QIODevice* const io);

    QIODevice* io();

public slots:
    void write(const QString& line);
    void close();
    void flush();

signals:
    void lineRead(const QString& cmd);
};

} // namespace protocol
} // namespace rainback

#endif // RAINBACK_PASCAL_HEADER

// vim: set ts=4 sw=4 :
