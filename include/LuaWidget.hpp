#ifndef RAINBACK_LUAWIDGET_HEADER
#define RAINBACK_LUAWIDGET_HEADER

#include <QWidget>
#include <lua-cxx/LuaEnvironment.hpp>
#include <lua-cxx/LuaValue.hpp>

class LuaWidget : public QWidget
{
    Q_OBJECT

    Lua& _lua;

protected:
    void paintEvent(QPaintEvent* const event);
    void wheelEvent(QWheelEvent* const event);
    void mousePressEvent(QMouseEvent* const event);
    void mouseReleaseEvent(QMouseEvent* const event);
    void mouseMoveEvent(QMouseEvent* const event);
public:
    LuaWidget(Lua& lua);
};

#endif // RAINBACK_LUAWIDGET_HEADER

// vim: set ts=4 sw=4 :
