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

public slots:
    void readData();

signals:
    void commandReceived(const QString& cmd);
};

} // namespace protocol
} // namespace rainback

#endif // RAINBACK_HUMAN_HEADER

// vim: set ts=4 sw=4 :
