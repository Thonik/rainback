#include "proxy/Painter.hpp"

#include "proxy/Font.hpp"

#include <lua-cxx/LuaEnvironment.hpp>
#include <lua-cxx/LuaException.hpp>
#include <lua-cxx/LuaStack.hpp>
#include <lua-cxx/LuaValue.hpp>

#include <QPaintDevice>

namespace rainback {
namespace proxy {

void Painter::initialize()
{
    // Set some high-quality defaults. I haven't benchmarked these
    // so I don't know how much they'll affect performance.
    painter()->setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter()->setRenderHint(QPainter::TextAntialiasing, true);

    setFontWeight(QFont::Normal);
}

void Painter::position(const qreal& x, const qreal& y)
{
    _x = x;
    _y = y;
}

void Painter::setFillColor(LuaStack& stack)
{
    setFillColor(
        stack.as<int>(1),
        stack.as<int>(2),
        stack.as<int>(3)
    );
    if (stack.size() == 4) {
        setFillAlpha(stack.as<int>(4));
    }
}

void Painter::setFillColor(const int r, const int g, const int b)
{
    QBrush brush(painter()->brush());

    QColor color(brush.color());
    color.setRed(r);
    color.setGreen(g);
    color.setBlue(b);
    brush.setColor(color);

    if (brush.style() == Qt::NoBrush) {
        brush.setStyle(Qt::SolidPattern);
    }
    painter()->setBrush(brush);
}

void Painter::setFillAlpha(const int alpha)
{
    QBrush brush(painter()->brush());

    QColor color(brush.color());
    color.setAlpha(alpha);
    brush.setColor(color);

    painter()->setBrush(brush);
}

void Painter::setPenColor(LuaStack& stack)
{
    setPenColor(
        stack.as<int>(1),
        stack.as<int>(2),
        stack.as<int>(3)
    );
    if (stack.size() == 4) {
        setPenAlpha(stack.as<int>(4));
    }
}

void Painter::setPenColor(const int r, const int g, const int b)
{
    QPen pen(painter()->pen());

    QColor color(pen.color());
    color.setRed(r);
    color.setGreen(g);
    color.setBlue(b);
    pen.setColor(color);

    painter()->setPen(pen);
}

void Painter::setPenAlpha(const int alpha)
{
    QPen pen(painter()->pen());

    QColor color(pen.color());
    color.setAlpha(alpha);
    pen.setColor(color);

    painter()->setPen(pen);
}

qreal Painter::penWidth() const
{
    return pen().width();
}

void Painter::setPenWidth(const qreal& width)
{
    QPen pen(painter()->pen());
    pen.setWidth(width);
    painter()->setPen(pen);
}

QString Painter::capStyle() const
{
    switch (pen().capStyle()) {
    case Qt::SquareCap:
        return "square";
    case Qt::FlatCap:
        return "flat";
    case Qt::RoundCap:
        return "round";
    default:
        return "";
    }
}

void Painter::setCapStyle(const QString& capStyle)
{
    QPen pen(painter()->pen());

    Qt::PenCapStyle capStyleValue;

    if (capStyle == "square") {
        capStyleValue = Qt::SquareCap;
    } else if (capStyle == "flat") {
        capStyleValue = Qt::FlatCap;
    } else if (capStyle == "round") {
        capStyleValue = Qt::RoundCap;
    } else {
        return;
    }

    pen.setCapStyle(capStyleValue);
    painter()->setPen(pen);
}

QString Painter::joinStyle() const
{
    switch (painter()->pen().joinStyle()) {
    case Qt::BevelJoin:
        return "bevel";
    case Qt::MiterJoin:
        return "miter";
    case Qt::RoundJoin:
        return "round";
    default:
        return "";
    }
}

void Painter::setJoinStyle(const QString& joinStyle)
{
    QPen pen(painter()->pen());

    Qt::PenJoinStyle joinStyleValue;

    if (joinStyle == "bevel") {
        joinStyleValue = Qt::BevelJoin;
    } else if (joinStyle == "miter") {
        joinStyleValue = Qt::MiterJoin;
    } else if (joinStyle == "round") {
        joinStyleValue = Qt::RoundJoin;
    } else {
        return;
    }

    pen.setJoinStyle(joinStyleValue);
    painter()->setPen(pen);
}

qreal Painter::miterLimit() const
{
    return pen().miterLimit();
}

void Painter::setMiterLimit(const qreal& limit)
{
    QPen pen(painter()->pen());
    pen.setMiterLimit(limit);
    painter()->setPen(pen);
}

void Painter::setOpacity(const qreal& opacity)
{
    painter()->setOpacity(opacity);
}

void Painter::setFont(LuaStack& stack)
{
    if (stack.size() == 1 && stack.type(1) == lua::Type::USERDATA) {
        std::shared_ptr<Font> font;
        stack >> font;
        if (font) {
            painter()->setFont(font->getFont());
            return;
        } else {
            throw LuaException("Userdata provided was not a font");
        }
    }
    if (stack.type(1) == lua::Type::STRING) {
        setFontFamily(stack.as<QString>(1));
    }
}

void Painter::setFontFamily(const QString& fontFamily)
{
    QFont font(painter()->font());

    font.setFamily(fontFamily);

    painter()->setFont(font);
}

void Painter::setFontWeight(const int weight)
{
    QFont font(painter()->font());

    font.setWeight(weight);

    painter()->setFont(font);
}

void Painter::setFontSize(const int fontSize)
{
    QFont font(painter()->font());

    font.setPointSize(fontSize);

    painter()->setFont(font);
}

void Painter::setItalic(const bool isItalic)
{
    QFont font(painter()->font());

    font.setItalic(isItalic);

    painter()->setFont(font);
}

void Painter::setBold(const bool isBold)
{
    QFont font(painter()->font());

    font.setBold(isBold);

    painter()->setFont(font);
}

void Painter::setUnderline(const bool isUnderline)
{
    QFont font(painter()->font());

    font.setUnderline(isUnderline);

    painter()->setFont(font);
}

void Painter::drawText(const QString& text)
{
    painter()->drawText(x(), y(), INT_MAX, INT_MAX, Qt::TextExpandTabs, text);
}

int Painter::textWidth(const QString& text)
{
    return painter()->fontMetrics().width(text);
}

int Painter::textHeight()
{
    return painter()->fontMetrics().height();
}

void Painter::drawPoint()
{
    painter()->drawPoint(QPointF(x(), y()));
}

void Painter::drawRect(const qreal& width, const qreal& height)
{
    painter()->drawRect(QRectF(x(), y(), width, height));
}

void Painter::drawRoundedRect(const qreal& width, const qreal& height, const qreal& xRadius, const qreal& yRadius)
{
    painter()->drawRoundedRect(QRectF(x(), y(), width, height), xRadius, yRadius);
}

void Painter::drawEllipse(const qreal& width, const qreal& height)
{
    painter()->drawEllipse(QRectF(x(), y(), width, height));
}

void Painter::drawArc(const qreal& width, const qreal& height, const int startAngle, const int spanAngle)
{
    painter()->drawArc(QRectF(x(), y(), width, height), startAngle, spanAngle);
}

void Painter::drawChord(const qreal& width, const qreal& height, const int startAngle, const int spanAngle)
{
    painter()->drawChord(QRectF(x(), y(), width, height), startAngle, spanAngle);
}

void Painter::drawPie(const qreal& width, const qreal& height, const int startAngle, const int spanAngle)
{
    painter()->drawPie(QRectF(x(), y(), width, height), startAngle, spanAngle);
}

void Painter::drawLine(const qreal& toX, const qreal& toY)
{
    painter()->drawLine(QPointF(x(), y()), QPointF(toX, toY));
}

QPolygonF getPolygonFromStack(Painter& painter, LuaStack& stack)
{
    LuaReference points(stack.saveAndPop());

    QPolygonF polygon;

    for (int i = 1; i <= points.length(); ++i) {
        auto point = points[i];
        polygon << QPointF(painter.x() + (qreal)point[1], painter.y() + (qreal)point[2]);
    }

    stack.clear();

    return polygon;
}

void Painter::drawPolygon(LuaStack& stack)
{
    painter()->drawPolygon(getPolygonFromStack(*this, stack));
}

void Painter::drawPolyline(LuaStack& stack)
{
    painter()->drawPolyline(getPolygonFromStack(*this, stack));
}

void Painter::drawPixmap(LuaStack& stack)
{
    switch (stack.size()) {
        case 1:
            painter()->drawPixmap(QPointF(x(), y()), QPixmap(stack.as<QString>(1)));
            break;
        case 3:
            {
                QPixmap image(stack.as<QString>(1));
                painter()->drawPixmap(
                    QRectF(x(), y(), stack.as<qreal>(2), stack.as<qreal>(3)),
                    image,
                    QRectF(0, 0, image.width(), image.height())
                );
            }
            break;
        case 7:
            {
                QPixmap image(stack.as<QString>(1));
                painter()->drawPixmap(
                    QRectF(x(), y(), stack.as<qreal>(2), stack.as<qreal>(3)),
                    image,
                    QRectF(
                        stack.as<qreal>(4), // sx
                        stack.as<qreal>(5), // sy
                        stack.as<qreal>(6), // sw
                        stack.as<qreal>(7)  // sh
                    )
                );
            }
            break;
    }

    stack.clear();
}

int Painter::width()
{
    return painter()->device()->width();
}

int Painter::height()
{
    return painter()->device()->height();
}

void Painter::reset()
{
    _x = _y = 0;
    painter()->end();
    painter()->begin(painter()->device());
    initialize();
}

} // namespace proxy
} // namespace rainback

// vim: set ts=4 sw=4 :
