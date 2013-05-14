#include "proxy/QObject.hpp"

namespace rainback {
namespace proxy {

void observeToDestroy(LuaStack& stack, QObject* obj, bool destroyOnGC)
{
    auto ptr = stack.as<LuaUserdata*>();
    if (!ptr->managed()) {
        return;
    }
    auto observer = new lua::QObjectObserver(obj, ptr);
    observer->setDestroyOnGC(true);
    return observer;
}

void wrapQObject(LuaStack& stack, QObject& obj, LuaReference& methods)
{
    auto userdata = stack.save();
    lua::qobject(stack, obj);

    methods["__methods"] = methods;

    auto worker = stack.lua()(""
        "return function(userdata, methods)\n"
        "    local mt = getmetatable(userdata);\n"
        ""
        "    local defaultIndex = mt.__index;\n"
        "    function mt.__index(self, key)\n"
        "        return methods[key] or defaultIndex(self, key);\n"
        "    end;\n"
        "end;\n"
    );
    worker(userdata, methods);
}

} // namespace proxy
} // namespace rainback

