#include "protocol/AbstractLine.hpp"

#include <iostream>
#include <cassert>
#include <QDataStream>

namespace rainback {
namespace protocol {

AbstractLine::AbstractLine() :
    _io(nullptr)
{
}

void AbstractLine::listen(QIODevice* const io)
{
    if (_io) {
        disconnect(_io, SIGNAL(destroyed()), this, SLOT(ioDestroyed()));
        disconnect(_io, SIGNAL(readyRead()), this, SLOT(flush()));
    }
    _io = io;
    if (!_io) {
        return;
    }
    connect(_io, SIGNAL(readyRead()), this, SLOT(flush()));
    connect(_io, SIGNAL(destroyed()), this, SLOT(ioDestroyed()));
    flush();
}

QIODevice* AbstractLine::io()
{
    return _io;
}

void AbstractLine::close()
{
    listen(nullptr);
}

void AbstractLine::ioDestroyed()
{
    _io = nullptr;
}

} // namespace protocol
} // namespace rainback
