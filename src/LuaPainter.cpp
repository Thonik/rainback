#include "LuaPainter.hpp"

#include "LuaFont.hpp"

#include <lua-cxx/LuaEnvironment.hpp>
#include <lua-cxx/LuaException.hpp>
#include <lua-cxx/LuaStack.hpp>
#include <lua-cxx/LuaValue.hpp>

#include <QPaintDevice>

void LuaPainter::initialize()
{
    // Set some high-quality defaults. I haven't benchmarked these
    // so I don't know how much they'll affect performance.
    painter()->setRenderHint(QPainter::Antialiasing, true);
    painter()->setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter()->setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter()->setRenderHint(QPainter::TextAntialiasing, true);

    setFontWeight(QFont::Normal);
}

void LuaPainter::position(const qreal& x, const qreal& y)
{
    _x = x;
    _y = y;
}

void LuaPainter::setFillColor(LuaStack& stack)
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

void LuaPainter::setFillColor(const int r, const int g, const int b)
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

void LuaPainter::setFillAlpha(const int alpha)
{
    QBrush brush(painter()->brush());

    QColor color(brush.color());
    color.setAlpha(alpha);
    brush.setColor(color);

    painter()->setBrush(brush);
}

void LuaPainter::setPenColor(LuaStack& stack)
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

void LuaPainter::setPenColor(const int r, const int g, const int b)
{
    QPen pen(painter()->pen());

    QColor color(pen.color());
    color.setRed(r);
    color.setGreen(g);
    color.setBlue(b);
    pen.setColor(color);

    painter()->setPen(pen);
}

void LuaPainter::setPenAlpha(const int alpha)
{
    QPen pen(painter()->pen());

    QColor color(pen.color());
    color.setAlpha(alpha);
    pen.setColor(color);

    painter()->setPen(pen);
}

qreal LuaPainter::penWidth() const
{
    return pen().width();
}

void LuaPainter::setPenWidth(const qreal& width)
{
    QPen pen(painter()->pen());
    pen.setWidth(width);
    painter()->setPen(pen);
}

QString LuaPainter::capStyle() const
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

void LuaPainter::setCapStyle(const QString& capStyle)
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

QString LuaPainter::joinStyle() const
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

void LuaPainter::setJoinStyle(const QString& joinStyle)
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

qreal LuaPainter::miterLimit() const
{
    return pen().miterLimit();
}

void LuaPainter::setMiterLimit(const qreal& limit)
{
    QPen pen(painter()->pen());
    pen.setMiterLimit(limit);
    painter()->setPen(pen);
}

void LuaPainter::setOpacity(const qreal& opacity)
{
    painter()->setOpacity(opacity);
}

void LuaPainter::setFont(LuaStack& stack)
{
    if (stack.size() == 1 && stack.type(1) == lua::Type::USERDATA) {
        std::shared_ptr<LuaFont> font;
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

void LuaPainter::setFontFamily(const QString& fontFamily)
{
    QFont font(painter()->font());

    font.setFamily(fontFamily);

    painter()->setFont(font);
}

void LuaPainter::setFontWeight(const int weight)
{
    QFont font(painter()->font());

    font.setWeight(weight);

    painter()->setFont(font);
}

void LuaPainter::setFontSize(const int fontSize)
{
    QFont font(painter()->font());

    font.setPointSize(fontSize);

    painter()->setFont(font);
}

void LuaPainter::setItalic(const bool isItalic)
{
    QFont font(painter()->font());

    font.setItalic(isItalic);

    painter()->setFont(font);
}

void LuaPainter::setBold(const bool isBold)
{
    QFont font(painter()->font());

    font.setBold(isBold);

    painter()->setFont(font);
}

void LuaPainter::setUnderline(const bool isUnderline)
{
    QFont font(painter()->font());

    font.setUnderline(isUnderline);

    painter()->setFont(font);
}

void LuaPainter::drawText(const QString& text)
{
    painter()->drawText(x(), y(), INT_MAX, INT_MAX, 0, text);
}

int LuaPainter::textWidth(const QString& text)
{
    return painter()->fontMetrics().width(text);
}

int LuaPainter::textHeight()
{
    return painter()->fontMetrics().height();
}

void LuaPainter::drawPoint()
{
    painter()->drawPoint(QPointF(x(), y()));
}

void LuaPainter::drawRect(const qreal& width, const qreal& height)
{
    painter()->drawRect(QRectF(x(), y(), width, height));
}

void LuaPainter::drawRoundedRect(const qreal& width, const qreal& height, const qreal& xRadius, const qreal& yRadius)
{
    painter()->drawRoundedRect(QRectF(x(), y(), width, height), xRadius, yRadius);
}

void LuaPainter::drawEllipse(const qreal& width, const qreal& height)
{
    painter()->drawEllipse(QRectF(x(), y(), width, height));
}

void LuaPainter::drawArc(const qreal& width, const qreal& height, const int startAngle, const int spanAngle)
{
    painter()->drawArc(QRectF(x(), y(), width, height), startAngle, spanAngle);
}

void LuaPainter::drawChord(const qreal& width, const qreal& height, const int startAngle, const int spanAngle)
{
    painter()->drawChord(QRectF(x(), y(), width, height), startAngle, spanAngle);
}

void LuaPainter::drawPie(const qreal& width, const qreal& height, const int startAngle, const int spanAngle)
{
    painter()->drawPie(QRectF(x(), y(), width, height), startAngle, spanAngle);
}

void LuaPainter::drawLine(const qreal& toX, const qreal& toY)
{
    painter()->drawLine(QPointF(x(), y()), QPointF(toX, toY));
}

QPolygonF getPolygonFromStack(LuaPainter& painter, LuaStack& stack)
{
    LuaValue points(stack.save());

    QPolygonF polygon;

    for (int i = 1; i <= points.length(); ++i) {
        auto point = points[i];
        polygon << QPointF(painter.x() + (qreal)point[1], painter.y() + (qreal)point[2]);
    }

    stack.clear();

    return polygon;
}

void LuaPainter::drawPolygon(LuaStack& stack)
{
    painter()->drawPolygon(getPolygonFromStack(*this, stack));
}

void LuaPainter::drawPolyline(LuaStack& stack)
{
    painter()->drawPolyline(getPolygonFromStack(*this, stack));
}

void LuaPainter::drawPixmap(LuaStack& stack)
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

int LuaPainter::width()
{
    return painter()->device()->width();
}

int LuaPainter::height()
{
    return painter()->device()->height();
}

void LuaPainter::reset()
{
    painter()->end();
    painter()->begin(painter()->device());
    initialize();
}

// vim: set ts=4 sw=4 :
