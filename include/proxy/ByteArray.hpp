#ifndef RAINBACK_PROXY_BYTEARRAY_HEADER
#define RAINBACK_PROXY_BYTEARRAY_HEADER

#include <QByteArray>

#include <lua-cxx/LuaStack.hpp>
#include <lua-cxx/userdata.hpp>
#include <lua-cxx/LuaEnvironment.hpp>
#include <lua-cxx/LuaValue.hpp>

namespace rainback {
namespace proxy {

void initializeByteArray(LuaStack& stack, QByteArray& array);

} // namespace proxy
} // namespace rainback

namespace lua {

template<>
struct UserdataType<QByteArray>
{
    constexpr static const char* name = "QByteArray";

    static void initialize(LuaStack& stack, QByteArray& array)
    {
        rainback::proxy::initializeByteArray(stack, array);
    }
};

} // namespace lua

#endif // RAINBACK_PROXY_BYTEARRAY_HEADER

// vim: set ts=4 sw=4 :
