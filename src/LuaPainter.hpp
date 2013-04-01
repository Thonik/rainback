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

    qreal _x;
    qreal _y;

    QPainter* painter()
    {
        return &_painter;
    }

    const QPainter* painter() const
    {
        return &_painter;
    }

    const QPen& pen() const
    {
        return painter()->pen();
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

    Q_PROPERTY(QString joinStyle READ joinStyle WRITE setJoinStyle)
    Q_PROPERTY(QString capStyle READ capStyle WRITE setCapStyle)
    Q_PROPERTY(qreal miterLimit READ miterLimit WRITE setMiterLimit)
    Q_PROPERTY(qreal penWidth READ penWidth WRITE setPenWidth)

public slots:

    void setFillColor(LuaStack& stack);
    void setFillColor(const int r, const int g, const int b);
    void setFillAlpha(const int alpha);

    void setPenColor(LuaStack& stack);
    void setPenColor(const int r, const int g, const int b);
    void setPenAlpha(const int alpha);

    void setPenWidth(const qreal& width);
    qreal penWidth() const;

    QString capStyle() const;
    void setCapStyle(const QString& capStyle);

    QString joinStyle() const;
    void setJoinStyle(const QString& joinStyle);

    double miterLimit() const;
    void setMiterLimit(const double& limit);

    void setOpacity(const double& opacity);

    void setFont(LuaStack& stack);
    void setFontFamily(const QString& fontFamily);
    void setFontSize(const int fontSize);
    void setFontWeight(const int weight);

    void setItalic(const bool isItalic);
    void setBold(const bool isItalic);
    void setUnderline(const bool isUnderline);

    void position(const qreal& x, const qreal& y);

    void drawText(const QString& text);
    int textWidth(const QString& text);
    int textHeight();

    void drawPoint();
    void drawLine(const qreal& toX, const qreal& toY);

    void drawRect(const qreal& width, const qreal& height);
    void drawRoundedRect(const qreal& width, const qreal& height, const qreal& xRadius, const qreal& yRadius);

    void drawEllipse(const qreal& width, const qreal& height);

    void drawArc(const qreal& width, const qreal& height, const int startAngle, const int spanAngle);
    void drawChord(const qreal& width, const qreal& height, const int startAngle, const int spanAngle);
    void drawPie(const qreal& width, const qreal& height, const int startAngle, const int spanAngle);

    void drawPolygon(LuaStack& stack);
    void drawPolyline(LuaStack& stack);

    void drawPixmap(LuaStack& stack);

    const qreal& x()
    {
        return _x;
    }

    const qreal& y()
    {
        return _y;
    }

    int width();

    int height();

    void reset();
};

#endif // RAINBACK_LUAPAINTER_HEADER

// vim: set ts=4 sw=4 :
