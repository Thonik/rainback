#include "LuaFont.hpp"
#include <lua-cxx/LuaStack.hpp>
#include <lua-cxx/userdata.hpp>

#include <QFontMetrics>

LuaFont::LuaFont() :
    _font()
{
}

LuaFont::LuaFont(const QString& fontFamily) :
    _font(fontFamily)
{
}

LuaFont::LuaFont(const QString& fontFamily, const int size) :
    _font(fontFamily, size)
{
}

const QFont& LuaFont::getFont() const
{
    return _font;
}

int LuaFont::height(LuaStack& stack)
{
    if (stack.size() == 1) {
        return height(stack.as<QString>(1), INT_MAX);
    }
    return height(stack.as<QString>(1), stack.as<int>(2));
}

int LuaFont::height(const QString& text, const int width)
{
    QFontMetrics metrics(getFont());

    return metrics.boundingRect(0, 0, width, INT_MAX, Qt::TextWordWrap, text).height();
}

LuaStack& operator<<(LuaStack& stack, const std::shared_ptr<LuaFont>& ptr)
{
    stack << LuaUserdata(ptr, "LuaFont");

    stack.pushMetatable();
    lua::userdata::qobject(stack, ptr.get());
    stack.setMetatable();
    return stack;
}

LuaStack& operator>>(LuaStack& stack, std::shared_ptr<LuaFont>& ptr)
{
    return lua::userdata::convertUserdata(stack, ptr, "LuaFont");
}

// vim: set ts=4 sw=4 :
