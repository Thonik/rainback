#include "Bootstrapper.hpp"

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

Bootstrapper::Bootstrapper() :
    _desktop(lua)
{
    fritomodLoader.setRoot(
        QDir(QDir::home().filePath("src/fritomod/fritomod"))
    );
    fritomodLoader.setPrefix("fritomod/");
    lua.addModuleLoader(&fritomodLoader);

    rainbackLoader.setRoot(QDir("../../rainback"));
    rainbackLoader.setPrefix("rainback/");
    lua.addModuleLoader(&rainbackLoader);

    wowLoader.setRoot(
        QDir(QDir::home().filePath("src/fritomod/wow"))
    );
    wowLoader.setPrefix("wow/");
    lua.addModuleLoader(&wowLoader);

    lua["Rainback"] = lua::value::table;
    lua["Rainback"]["Font"] = newFont;

    lua["Rainback" ]["ScreenWidth"] = std::function<int()>([this]() {
        return _desktop.width();
    });

    lua["Rainback" ]["ScreenHeight"] = std::function<int()>([this]() {
        return _desktop.height();
    });

    // Load WoW compatibility methods
    lua::load_dir(lua, QDir("../../wow"), true);

    lua::load_dir(lua, QDir("../../scripts"), true);
}

QWidget& Bootstrapper::mainWidget()
{
    return _desktop;
}

// vim: set ts=4 sw=4 :
