#ifndef RAINBACK_LUAFONT_HEADER
#define RAINBACK_LUAFONT_HEADER

#include <QObject>
#include <QFont>
#include <memory>

#include <lua-cxx/LuaStack.hpp>
#include <lua-cxx/userdata.hpp>

class LuaFont : public QObject
{
    Q_OBJECT

    QFont _font;
public:
    LuaFont();
    LuaFont(const QString& fontFamily);
    LuaFont(const QString& fontFamily, const int size);

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

namespace lua {

template<>
struct UserdataType<LuaFont>
{
    constexpr static const char* name = "LuaFont";

    static void initialize(LuaStack& stack, LuaFont& font)
    {
        lua::userdata::qobject(stack, font);
    }
};

} // namespace lua

#endif // RAINBACK_LUAFONT_HEADER

// vim: set ts=4 sw=4 :
