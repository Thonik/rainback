#include "Desktop.hpp"

#include <QPainter>
#include <functional>

#include <lua-cxx/LuaValue.hpp>
#include <lua-cxx/loaders.hpp>
#include <lua-cxx/userdata.hpp>

#include "LuaPainter.hpp"

Desktop::Desktop()
{
    fritomodLoader.setRoot(QDir(QDir::home().filePath("src/fritomod/fritomod")));
    fritomodLoader.setPrefix("fritomod/");

    rainbackLoader.setRoot(QDir("../../rainback"));
    rainbackLoader.setPrefix("rainback/");

    lua.addModuleLoader(&fritomodLoader);
    lua.addModuleLoader(&rainbackLoader);

    lua::load_dir(lua, QDir("../../scripts"), true);

    QPalette p(palette());
    p.setColor(QPalette::Background, QColor(255, 255, 221));
    setAutoFillBackground(true);
    setPalette(p);
}

void Desktop::paintEvent(QPaintEvent* event)
{
    lua["paint"](LuaPainter(this));
}

// vim: set ts=4 sw=4 :
