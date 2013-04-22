#ifndef RAINBACK_BOOTSTRAPPER_HEADER
#define RAINBACK_BOOTSTRAPPER_HEADER

#include "LuaWidget.hpp"
#include "Rainback.hpp"

#include <vector>
#include <memory>

#include <lua-cxx/LuaEnvironment.hpp>
#include <lua-cxx/DirectoryModuleLoader.hpp>

#include <QWidget>

class Bootstrapper
{
    Lua _lua;
    LuaWidget _desktop;
    rainback::Rainback _rainback;

public:
    Bootstrapper();

    QWidget& mainWidget();
};

#endif // RAINBACK_BOOTSTRAPPER_HEADER


// vim: set ts=4 sw=4 :
