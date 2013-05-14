#ifndef RAINBACK_PROXY_QOBJECT_HEADER
#define RAINBACK_PROXY_QOBJECT_HEADER

#include <QObject>

#include <lua-cxx/QObjectObserver.hpp>
#include <lua-cxx/LuaStack.hpp>
#include <lua-cxx/LuaEnvironment.hpp>
#include <lua-cxx/LuaValue.hpp>

namespace rainback {
namespace proxy {

void observeToDestroy(LuaStack& stack, QObject& obj, bool destroyOnGC);
void wrapQObject(LuaStack& stack, QObject& obj, LuaReference& methods);

} // namespace proxy
} // namespace rainback

#endif // RAINBACK_PROXY_QOBJECT_HEADER
