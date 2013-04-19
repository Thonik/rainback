#ifndef RAINBACK_BOOTSTRAPPER_HEADER
#define RAINBACK_BOOTSTRAPPER_HEADER

#include "Desktop.hpp"

#include <lua-cxx/DirectoryModuleLoader.hpp>

#include <QElapsedTimer>
#include <QBasicTimer>

class Bootstrapper
{
    Desktop _desktop;

    Lua lua;
    DirectoryModuleLoader fritomodLoader;
    DirectoryModuleLoader wowLoader;
    DirectoryModuleLoader rainbackLoader;

    QElapsedTimer elapsed;
    QBasicTimer timer;

public:
    Bootstrapper();

    QWidget& mainWidget();
};

#endif // RAINBACK_BOOTSTRAPPER_HEADER


// vim: set ts=4 sw=4 :
