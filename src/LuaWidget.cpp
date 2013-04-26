#include "LuaWidget.hpp"

#include "LuaPainter.hpp"
#include "Rainback.hpp"

#include <QWheelEvent>

LuaWidget::LuaWidget(Lua& lua) :
    _lua(lua)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
}


void LuaWidget::paintEvent(QPaintEvent* const event)
{
    rainback::dispatch(_lua, "RENDER", LuaPainter(this));
}

void LuaWidget::wheelEvent(QWheelEvent* const event)
{
    rainback::dispatch(_lua, "MOUSEWHEEL", event->delta());
}

std::string buttonName(const Qt::MouseButton& button)
{
    switch (button) {
        case Qt::LeftButton:
            return "LeftButton";
        case Qt::RightButton:
            return "RightButton";
        case Qt::MiddleButton:
            return "MiddleButton";
        case Qt::XButton1:
            return "Button4";
        case Qt::XButton2:
            return "Button5";
        case Qt::NoButton:
            throw std::logic_error("No button pressed");
        default:
            std::stringstream msg;
            msg << "Unexpected button: ";
            msg << button;
            throw std::logic_error(msg.str());
    }
}

std::string keyName(const int& key)
{
    switch (key) {
        case Qt::Key_Escape:
            return "ESCAPE";
        default:
            return "UNKNOWN";
    }
}

void LuaWidget::mousePressEvent(QMouseEvent* const event)
{
    rainback::dispatch(_lua, "MOUSEPRESS", buttonName(event->button()), event->x(), event->y());
}

void LuaWidget::mouseReleaseEvent(QMouseEvent* const event)
{
    rainback::dispatch(_lua, "MOUSERELEASE", buttonName(event->button()), event->x(), event->y());
}

void LuaWidget::focusInEvent(QFocusEvent* const event)
{
    rainback::dispatch(_lua, "FOCUSIN");
}

void LuaWidget::keyPressEvent(QKeyEvent* const event)
{
    rainback::dispatch(_lua, "KEYPRESS", keyName(event->key()));
}

void LuaWidget::keyReleaseEvent(QKeyEvent* const event)
{
    rainback::dispatch(_lua, "KEYRELEASE", keyName(event->key()));
}

void LuaWidget::focusOutEvent(QFocusEvent* const event)
{
    rainback::dispatch(_lua, "FOCUSOUT");
}

void LuaWidget::mouseMoveEvent(QMouseEvent* const event)
{
    rainback::dispatch(_lua, "MOUSEMOVE", event->x(), event->y());
}

// vim: set ts=4 sw=4 :
