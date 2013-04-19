#ifndef RAINBACK_DESKTOP_HEADER
#define RAINBACK_DESKTOP_HEADER

#include <QWidget>
#include <lua-cxx/LuaEnvironment.hpp>

class Desktop : public QWidget
{
    Q_OBJECT

    Lua& lua;

protected:
    void paintEvent(QPaintEvent* event);

public:
    Desktop(Lua& lua);
};

#endif // RAINBACK_DESKTOP_HEADER

// vim: set ts=4 sw=4 :
