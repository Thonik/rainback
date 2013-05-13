#ifndef RAINBACK_PROXY_PASCAL_HEADER
#define RAINBACK_PROXY_PASCAL_HEADER

#include "proxy/QObject.hpp"
#include "proxy/Socket.hpp"
#include "protocol/Pascal.hpp"

namespace lua {

template<>
struct UserdataType<rainback::protocol::Pascal>
{
    constexpr static const char* name = "rainback::protocol::Pascal";

    static void initialize(LuaStack& stack, rainback::protocol::Pascal& ptl)
    {
        lua::push(stack, lua::value::table);
        auto methods = stack.save();

        methods["listen"] = std::function<void(rainback::protocol::Pascal&, QAbstractSocket*)>(
            [](rainback::protocol::Pascal& ptl, QAbstractSocket* obj) {
                auto socket = qobject_cast<QAbstractSocket*>(obj);
                if (!socket) {
                    throw LuaException("A socket or other IO device must be provided");
                }
                ptl.listen(socket);
            }
        );

        methods["write"] = std::function<void(rainback::protocol::Pascal&, const QString&)>(
            [](rainback::protocol::Pascal& ptl, const QString& data) {
                if (!ptl.io()) {
                    throw LuaException("Write must have a socket or IO device to write to");
                }
                auto io = ptl.io();
                if (!io->isOpen()) {
                    throw LuaException("Write cannot write to a closed socket or IO device");
                }
                if (qobject_cast<QAbstractSocket*>(io)) {
                    auto socket = qobject_cast<QAbstractSocket*>(io);
                    if (socket->state() != QAbstractSocket::ConnectedState) {
                        std::stringstream str;
                        str << "Write cannot write to an unconnected socket. Current state was ";
                        str << rainback::proxy::getSocketStateName(socket->state());
                        throw LuaException(str.str());
                    }
                }
                ptl.write(data);
            }
        );

        rainback::proxy::wrapQObject(stack, ptl, methods);
    }
};

} // namespace lua

#endif // RAINBACK_PROXY_PASCAL_HEADER
