#include "Rainback.hpp"

#include <functional>

#include <lua-cxx/LuaValue.hpp>
#include <lua-cxx/loaders.hpp>
#include <lua-cxx/userdata.hpp>

#include "LuaPainter.hpp"
#include "LuaFont.hpp"

std::shared_ptr<LuaFont> newFont(LuaStack& stack)
{
    switch (stack.size()) {
    case 0:
        return std::make_shared<LuaFont>();
    case 1:
        return std::make_shared<LuaFont>(stack.as<QString>(1));
    default:
        return std::make_shared<LuaFont>(
            stack.as<QString>(1),
            stack.as<int>(2)
        );
    }
}

namespace rainback {

void Rainback::assertWidget()
{
    if (!_widget) {
        throw LuaException("This function must not be used when no widget is available");
    }
}

Rainback::Rainback(Lua& lua) :
    _widget(nullptr),
    _lua(lua)
{
    _lua["Rainback"] = lua::value::table;

    _lua["Rainback"]["AddModuleDirectory"] =
    std::function<void(const std::string&, const std::string&)>(
        [this](const std::string& root, const std::string& prefix) {
            auto loader = std::make_shared<DirectoryModuleLoader>();
            loaders.push_back(loader);
            loader->setRoot(QDir(root.c_str()));
            loader->setPrefix(prefix);
            _lua.addModuleLoader(loader.get());
        }
    );

    _lua["Rainback"]["LoadDirectory"] = std::function<void(const std::string&, const bool)>(
        [this](const std::string& path, const bool recurse) {
            lua::load_dir(_lua, QDir(path.c_str()), recurse);
        }
    );

    _lua["Rainback"]["Font"] = newFont;

    _lua["Rainback" ]["ScreenWidth"] = std::function<int()>([this]() {
        assertWidget();
        return _widget->width();
    });

    _lua["Rainback" ]["ScreenHeight"] = std::function<int()>([this]() {
        assertWidget();
        return _widget->height();
    });

    _lua["Rainback"]["GetTime"] = std::function<double()>([this]() {
        return elapsed.elapsed() / 1000;
    });

    _lua["Rainback"]["StartTick"] =
    std::function<std::function<void()>(const int)>([this](const int framerate) {
        timer.start(1000 / framerate);
        return [this]() {
            timer.stop();
        };
    });

    _lua["Rainback"]["GetCursorPosition"] =
    std::function<void(LuaStack&)>([this](LuaStack& stack) {
        assertWidget();
        QPoint pos(_widget->mapFromGlobal(QCursor::pos()));
        stack.clear();
        lua::push(stack, pos.x());
        lua::push(stack, pos.y());
    });

    _lua["Rainback"]["Update"] = std::function<void()>([this]() {
        assertWidget();
        _widget->update();
    });

    elapsed.start();

    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void Rainback::timeout()
{
    rainback::dispatch(_lua, "UPDATE");
}

} // namespace rainback

// vim: set ts=4 sw=4 :
