#include "proxy/Socket.hpp"

namespace rainback {
namespace proxy {

std::string getSocketStateName(const QAbstractSocket::SocketState state) {
    switch (state) {
        case QAbstractSocket::UnconnectedState:
            return "unconnected";
        case QAbstractSocket::HostLookupState:
            return "hostlookup";
        case QAbstractSocket::ConnectingState:
            return "connecting";
        case QAbstractSocket::ConnectedState:
            return "connected";
        case QAbstractSocket::BoundState:
            return "bound";
        case QAbstractSocket::ClosingState:
            return "closing";
        default:
            std::stringstream str;
            str << "unknown(" << state << ")";
            return str.str();
    }
}

void validatePort(const int port) {
    if (port > 0 && port <= USHRT_MAX) {
        return;
    }
    std::stringstream str;
    if (port < 0) {
        str << "Port must be greater than zero, but was given " << port;
    } else if (port > USHRT_MAX) {
        str << "Port must be less than " << USHRT_MAX << ", but was given " << port;
    }
    throw LuaException(str.str());
}

} // namespace proxy
} // namespace rainback

