#ifndef RAINBACK_DESKTOP_HEADER
#define RAINBACK_DESKTOP_HEADER

#include <QWidget>
#include <lua-cxx/LuaEnvironment.hpp>
#include <lua-cxx/DirectoryModuleLoader.hpp>

class Desktop : public QWidget
{
    Q_OBJECT

    Lua lua;
    DirectoryModuleLoader fritomodLoader;
    DirectoryModuleLoader rainbackLoader;

protected:
    void paintEvent(QPaintEvent* event);

public:
    Desktop();
};

#endif // RAINBACK_DESKTOP_HEADER

// vim: set ts=4 sw=4 :
