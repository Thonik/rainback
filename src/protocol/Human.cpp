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
        disconnect(_io, SIGNAL(readyRead()), this, SLOT(flush()));
    }
    _io = io;
    if (!_io) {
        return;
    }
    connect(_io, SIGNAL(readyRead()), this, SLOT(flush()));
}

void Human::close()
{
    listen(nullptr);
}

void Human::write(const QString& line)
{
    _io->write(line.toUtf8());
}

void Human::flush()
{
    while (_io && _io->bytesAvailable() > 1 && _io->canReadLine()) {
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
        emit lineRead(line);
    }
}

} // namespace protocol
} // namespace rainback
