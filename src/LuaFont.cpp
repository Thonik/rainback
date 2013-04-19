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

QString LuaFont::family() const
{
    return _font.family();
}

void LuaFont::setFamily(const QString& font)
{
    _font.setFamily(font);
}

int LuaFont::pointSize() const
{
    return _font.pointSize();
}

void LuaFont::setPointSize(const int pointSize)
{
    _font.setPointSize(pointSize);
}

int LuaFont::width(const QString& text) const
{
    QFontMetrics metrics(getFont());

    return metrics.boundingRect(0, 0, INT_MAX, INT_MAX, Qt::TextWordWrap, text).width();
}

int LuaFont::height(LuaStack& stack) const
{
    int width = INT_MAX;
    if (stack.size() == 2 && !stack.isNil(2)) {
        width = stack.as<int>(2);
    }
    return height(stack.as<QString>(1), width);
}

int LuaFont::height(const QString& text, const int width) const
{
    QFontMetrics metrics(getFont());

    return metrics.boundingRect(0, 0, width, INT_MAX, Qt::TextWordWrap, text).height();
}

// vim: set ts=4 sw=4 :
