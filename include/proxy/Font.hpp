#ifndef RAINBACK_PROXY_FONT_HEADER
#define RAINBACK_PROXY_FONT_HEADER

#include <QObject>
#include <QFont>
#include <memory>

#include <lua-cxx/LuaStack.hpp>
#include <lua-cxx/userdata.hpp>

namespace rainback {
namespace proxy {

class Font : public QObject
{
    Q_OBJECT

    QFont _font;
public:
    Font();
    Font(const QString& fontFamily);
    Font(const QString& fontFamily, const int size);

    const QFont& getFont() const;

    Q_PROPERTY(QString family READ family WRITE setFamily)
    Q_PROPERTY(int pointSize READ pointSize WRITE setPointSize)

public slots:
    QString family() const;
    void setFamily(const QString& font);

    int pointSize() const;
    void setPointSize(const int size);

    int height(LuaStack& stack) const;
    int height(const QString& text, const int width) const;

    int width(const QString& text) const;
};

} // namespace proxy
} // namespace rainback

namespace lua {

template<>
struct UserdataType<rainback::proxy::Font>
{
    constexpr static const char* name = "rainback::proxy::Font";

    static void initialize(LuaStack& stack, rainback::proxy::Font& font)
    {
        lua::UserdataType<QObject>::initialize(stack, font);
    }
};

} // namespace lua

#endif // RAINBACK_PROXY_FONT_HEADER

// vim: set ts=4 sw=4 :
