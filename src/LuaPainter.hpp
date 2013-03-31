#ifndef RAINBACK_LUAPAINTER_HEADER
#define RAINBACK_LUAPAINTER_HEADER

#include <QPainter>
#include <QObject>

class LuaStack;
class Lua;

class LuaPainter : public QObject
{
    Q_OBJECT

    QPainter _painter;

    int _x;
    int _y;

    QPainter* painter()
    {
        return &_painter;
    }

public:

    LuaPainter(QPaintDevice* const device) :
        _painter(device),
        _x(0),
        _y(0)
    {
        initialize();
    }

    void initialize();

public slots:

    void setFillColor(LuaStack& stack);
    void setFillColor(const int r, const int g, const int b);
    void setFillAlpha(const int alpha);

    void setPenColor(LuaStack& stack);
    void setPenColor(const int r, const int g, const int b);
    void setPenAlpha(const int alpha);

    void setPenWidth(const int width);
    void setCapStyle(const QString& capStyle);
    void setJoinStyle(const QString& joinStyle);

    void setOpacity(const double& opacity);

    void setFontFamily(const QString& fontFamily);
    void setFontSize(const int fontSize);
    void setFontWeight(const int weight);

    void setItalic(const bool isItalic);
    void setBold(const bool isItalic);
    void setUnderline(const bool isUnderline);

    void position(const int x, const int y);

    void drawText(const QString& text);

    void drawPoint();
    void drawLine(const int toX, const int toY);

    void drawRect(const int width, const int height);
    void drawRoundedRect(const int width, const int height, const int xRadius, const int yRadius);

    void drawEllipse(const int width, const int height);

    void drawArc(const int width, const int height, const int startAngle, const int spanAngle);
    void drawChord(const int width, const int height, const int startAngle, const int spanAngle);
    void drawPie(const int width, const int height, const int startAngle, const int spanAngle);

    void drawPolygon(LuaStack& stack);
    void drawPolyline(LuaStack& stack);

    void drawPixmap(LuaStack& stack);

    int x()
    {
        return _x;
    }

    int y()
    {
        return _y;
    }

    int width();

    int height();

    void reset();
};

#endif // RAINBACK_LUAPAINTER_HEADER

// vim: set ts=4 sw=4 :
