#include "proxy/Font.hpp"
#include <lua-cxx/LuaStack.hpp>
#include <lua-cxx/userdata.hpp>

#include <QFontMetrics>

namespace rainback {
namespace proxy {

Font::Font() :
    _font()
{
}

Font::Font(const QString& fontFamily) :
    _font(fontFamily)
{
}

Font::Font(const QString& fontFamily, const int size) :
    _font(fontFamily, size)
{
}

const QFont& Font::getFont() const
{
    return _font;
}

QString Font::family() const
{
    return _font.family();
}

void Font::setFamily(const QString& font)
{
    _font.setFamily(font);
}

int Font::pointSize() const
{
    return _font.pointSize();
}

void Font::setPointSize(const int pointSize)
{
    _font.setPointSize(pointSize);
}

int Font::width(const QString& text) const
{
    QFontMetrics metrics(getFont());

    return metrics.boundingRect(0, 0, INT_MAX, INT_MAX, Qt::TextWordWrap, text).width();
}

int Font::height(LuaStack& stack) const
{
    int width = INT_MAX;
    if (stack.size() == 2 && !stack.isNil(2)) {
        width = stack.as<int>(2);
    }
    return height(stack.as<QString>(1), width);
}

int Font::height(const QString& text, const int width) const
{
    QFontMetrics metrics(getFont());

    return metrics.boundingRect(0, 0, width, INT_MAX, Qt::TextWordWrap, text).height();
}

} // namespace proxy
} // namespace rainback

// vim: set ts=4 sw=4 :
