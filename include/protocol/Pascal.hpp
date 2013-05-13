#ifndef RAINBACK_PASCAL_HEADER
#define RAINBACK_PASCAL_HEADER

#include "protocol/AbstractLine.hpp"

namespace rainback {
namespace protocol {

class Pascal : public AbstractLine
{
    Q_OBJECT

    typedef quint32 packetsize;
    packetsize lineSize;

public:
    Pascal();

public slots:
    void write(const QString& line);
    void flush();
};

} // namespace protocol
} // namespace rainback

#endif // RAINBACK_PASCAL_HEADER

// vim: set ts=4 sw=4 :
