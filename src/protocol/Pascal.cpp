#include "protocol/Pascal.hpp"

#include <QDataStream>
#include <cassert>
#include <iostream>

namespace rainback {
namespace protocol {

Pascal::Pascal() :
    lineSize(0), AbstractLine()
{
}

void Pascal::write(const QString& line)
{
    assert(_io);

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_0);

    stream << static_cast<packetsize>(0);

    stream << line;

    stream.device()->seek(0);
    const packetsize writtenSize(static_cast<packetsize>(data.size() - sizeof(packetsize)));
    stream << writtenSize;

    _io->write(data);
}

void Pascal::flush()
{
    if (!_io) {
        return;
    }

    QDataStream stream(_io);
    stream.setVersion(QDataStream::Qt_4_0);

    while (_io && _io->bytesAvailable()) {
        if (!lineSize) {
            if (_io->bytesAvailable() < static_cast<int>(sizeof(packetsize))) {
                return;
            }
            stream >> lineSize;
            if (!lineSize) {
                // Line size is zero. Weird, but just carry on.
                continue;
            }
        }

        if (_io->bytesAvailable() < lineSize) {
            return;
        }
        lineSize = 0;

        QString line;
        stream >> line;

        std::cout << "Received " << line.length() << " byte(s): " << line.toStdString() << std::endl;
        emit lineRead(line);
    }
}

} // namespace protocol
} // namespace rainback
