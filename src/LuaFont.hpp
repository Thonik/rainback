#ifndef RAINBACK_LUAFONT_HEADER
#define RAINBACK_LUAFONT_HEADER

#include <QObject>
#include <QFont>
#include <memory>

class LuaStack;

class LuaFont : public QObject
{
    Q_OBJECT

    QFont _font;
public:
    LuaFont();
    LuaFont(const QString& fontFamily);
    LuaFont(const QString& fontFamily, const int size);

    const QFont& getFont() const;

public slots:

    int height(LuaStack& stack);
    int height(const QString& text, const int width);
};

LuaStack& operator>>(LuaStack& stack, std::shared_ptr<LuaFont>& ptr);
LuaStack& operator<<(LuaStack& stack, const std::shared_ptr<LuaFont>& ptr);

#endif // RAINBACK_LUAFONT_HEADER

// vim: set ts=4 sw=4 :
