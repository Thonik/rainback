#include "proxy/QObject.hpp"

namespace rainback {
namespace proxy {

void observeToDestroy(LuaStack& stack, QObject& obj, bool destroyOnGC)
{
    auto ptr = stack.as<LuaUserdata*>();
    if (!ptr->managed()) {
        return;
    }
    auto observer = new lua::QObjectObserver(&obj, ptr);
    observer->setDestroyOnGC(true);
}

void wrapQObject(LuaStack& stack, QObject& obj, LuaReference& extra)
{
    auto userdata = stack.save();
    lua::qobject(stack, obj);

    extra["__extra"] = extra;

    auto worker = stack.lua()(""
        "return function(userdata, extra)\n"
        "    local mt = getmetatable(userdata);\n"
        ""
        "    local defaultIndex = mt.__index;\n"
        "    function mt.__index(self, key)\n"
        "        return extra[key] or defaultIndex(self, key);\n"
        "    end;\n"
        "end;\n"
    );
    worker(userdata, extra);
}

} // namespace proxy
} // namespace rainback

