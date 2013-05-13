#include "protocol/Human.hpp"

#include <iostream>
#include <cassert>

namespace rainback {
namespace protocol {

Human::Human() :
    _io(nullptr)
{
}

void Human::listen(QIODevice* const io)
{
    if (_io) {
        disconnect(_io, SIGNAL(readyRead()), this, SLOT(readData()));
    }
    _io = io;
    if (!_io) {
        return;
    }
    connect(_io, SIGNAL(readyRead()), this, SLOT(readData()));
}

void Human::readData()
{
    while (_io->bytesAvailable() > 1 && _io->canReadLine()) {
        // Add 1 for the \0 terminator
        QByteArray lineData = _io->readLine(_io->bytesAvailable() + 1);
        assert(lineData.size() > 0);

        QString line(lineData);

        line = line.trimmed();

        if (line.isEmpty()) {
            // Discard empty lines
            continue;
        }

        if (line.startsWith("#")) {
            // Ignore comments
            continue;
        }

        std::cout << "Received " << line.length() << " byte(s): " << line.toStdString() << std::endl;
        emit commandReceived(line);
    }
}

} // namespace protocol
} // namespace rainback

