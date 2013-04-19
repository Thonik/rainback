#ifndef RAINBACK_BOOTSTRAPPER_HEADER
#define RAINBACK_BOOTSTRAPPER_HEADER

#include "Desktop.hpp"

#include <lua-cxx/DirectoryModuleLoader.hpp>

class Bootstrapper
{
    Desktop _desktop;

    Lua lua;
    DirectoryModuleLoader fritomodLoader;
    DirectoryModuleLoader wowLoader;
    DirectoryModuleLoader rainbackLoader;

public:
    Bootstrapper();

    QWidget& mainWidget();
};

#endif // RAINBACK_BOOTSTRAPPER_HEADER


// vim: set ts=4 sw=4 :
