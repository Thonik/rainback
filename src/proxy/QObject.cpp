#include "proxy/QObject.hpp"

namespace rainback {
namespace proxy {

void observeToDestroy(QObject* obj, LuaUserdata* userdata)
{
    auto observer = new lua::QObjectObserver(obj, userdata);
    observer->setDestroyOnGC(true);
}

void wrapQObject(LuaStack& stack, QObject& obj, LuaReference& methods)
{
    auto userdata = stack.save();
    lua::push(stack, userdata);
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

    observeToDestroy(&obj, userdata.as<LuaUserdata*>());
}

} // namespace proxy
} // namespace rainback

