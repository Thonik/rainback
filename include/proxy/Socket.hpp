#ifndef RAINBACK_PROXY_SOCKET_HEADER
#define RAINBACK_PROXY_SOCKET_HEADER

#include <iostream>
#include <string>

#include <lua-cxx/LuaStack.hpp>
#include <lua-cxx/LuaEnvironment.hpp>
#include <lua-cxx/LuaValue.hpp>

#include <QAbstractSocket>

#include "proxy/QObject.hpp"

namespace rainback {
namespace proxy {

std::string getSocketStateName(const QAbstractSocket::SocketState state);
void validatePort(const int port);

} // namespace proxy
} // namespace rainback

namespace lua {

template<>
struct UserdataType<QAbstractSocket>
{
    constexpr static const char* name = "Socket";

    static void initialize(LuaStack& stack, QAbstractSocket& socket)
    {
        lua::push(stack, lua::value::table);
        auto methods = stack.save();

        methods["state"] = std::function<std::string(QAbstractSocket& socket)>(
            [](QAbstractSocket& socket) {
                return rainback::proxy::getSocketStateName(socket.state());
            }
        );

        rainback::proxy::wrapQObject(stack, socket, methods);
    }
};

} // namespace lua

#endif // RAINBACK_PROXY_SOCKET_HEADER

// vim: set ts=4 sw=4 :
