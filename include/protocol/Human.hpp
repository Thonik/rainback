#ifndef RAINBACK_HUMAN_HEADER
#define RAINBACK_HUMAN_HEADER

#include "protocol/AbstractLine.hpp"

namespace rainback {
namespace protocol {

class Human : public AbstractLine
{
    Q_OBJECT

public slots:
    void write(const QString& line);
    void flush();
};

} // namespace protocol
} // namespace rainback

#endif // RAINBACK_HUMAN_HEADER

// vim: set ts=4 sw=4 :
