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

LuaStack& operator>>(LuaStack& stack, std::shared_ptr<LuaFont>& ptr);
LuaStack& operator<<(LuaStack& stack, const std::shared_ptr<LuaFont>& ptr);

#endif // RAINBACK_LUAFONT_HEADER

// vim: set ts=4 sw=4 :
