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

void LuaPainter::position(const int x, const int y)
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

void LuaPainter::setPenWidth(const int width)
{
    QPen pen(painter()->pen());
    pen.setWidth(width);
    painter()->setPen(pen);
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

void LuaPainter::setOpacity(const double& opacity)
{
    painter()->setOpacity(opacity);
}

#include <iostream>

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
    painter()->drawText(x(), y(), text);
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
    painter()->drawPoint(x(), y());
}

void LuaPainter::drawRect(const int width, const int height)
{
    painter()->drawRect(x(), y(), width, height);
}

void LuaPainter::drawRoundedRect(const int width, const int height, const int xRadius, const int yRadius)
{
    painter()->drawRoundedRect(x(), y(), width, height, xRadius, yRadius);
}

void LuaPainter::drawEllipse(const int width, const int height)
{
    painter()->drawEllipse(x(), y(), width, height);
}

void LuaPainter::drawArc(const int width, const int height, const int startAngle, const int spanAngle)
{
    painter()->drawArc(x(), y(), width, height, startAngle, spanAngle);
}

void LuaPainter::drawChord(const int width, const int height, const int startAngle, const int spanAngle)
{
    painter()->drawChord(x(), y(), width, height, startAngle, spanAngle);
}

void LuaPainter::drawPie(const int width, const int height, const int startAngle, const int spanAngle)
{
    painter()->drawPie(x(), y(), width, height, startAngle, spanAngle);
}

void LuaPainter::drawLine(const int toX, const int toY)
{
    painter()->drawLine(x(), y(), toX, toY);
}

QPolygon getPolygonFromStack(LuaPainter& painter, LuaStack& stack)
{
    LuaValue points(stack.save());

    QPolygon polygon;

    for (int i = 1; i <= points.length(); ++i) {
        auto point = points[i];
        polygon << QPoint(painter.x() + (int)point[1], painter.y() + (int)point[2]);
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
            painter()->drawPixmap(x(), y(), QPixmap(stack.as<QString>(1)));
            break;
        case 3:
            painter()->drawPixmap(x(), y(), stack.as<int>(2), stack.as<int>(3), QPixmap(stack.as<QString>(1)));
            break;
        case 7:
            painter()->drawPixmap(x(), y(),
                stack.as<int>(2), stack.as<int>(3),
                QPixmap(stack.as<QString>(1)),
                stack.as<int>(4), // sx
                stack.as<int>(5), // sy
                stack.as<int>(6), // sw
                stack.as<int>(7)  // sh
            );
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
