#include "Bootstrapper.hpp"

#include <functional>

#include <lua-cxx/LuaValue.hpp>
#include <lua-cxx/loaders.hpp>
#include <lua-cxx/userdata.hpp>

#include "LuaPainter.hpp"
#include "LuaFont.hpp"

Bootstrapper::Bootstrapper() :
    _lua(),
    _desktop(_lua),
    _rainback(_lua)
{
    _rainback.setWidget(&_desktop);
    QFile file("../../demo/init.lua");
    _lua(file);
}

QWidget& Bootstrapper::mainWidget()
{
    return _desktop;
}

// vim: set ts=4 sw=4 :
