#ifndef RAINBACK_HUMAN_HEADER
#define RAINBACK_HUMAN_HEADER

#include <QObject>
#include <QIODevice>

namespace rainback {
namespace protocol {

class Human : public QObject
{
    Q_OBJECT
    QIODevice* _io;

public:
    Human();
    void listen(QIODevice* const io);
    void close();

public slots:
    void write(const QString& line);
    void flush();

signals:
    void lineRead(const QString& cmd);
};

} // namespace protocol
} // namespace rainback

#endif // RAINBACK_HUMAN_HEADER

// vim: set ts=4 sw=4 :
