#include "Desktop.hpp"

#include "LuaPainter.hpp"

#include <QWheelEvent>

Desktop::Desktop(Lua& lua) :
    _lua(lua)
{
    QPalette p(palette());
    p.setColor(QPalette::Background, QColor(255, 255, 221));
    setAutoFillBackground(true);
    setPalette(p);

    setMouseTracking(true);
}


template <typename... Args>
void dispatch(Lua& lua, std::string event, Args&&... args)
{
    auto dispatcher = lua["Rainback"]["DispatchEvent"];
    if (dispatcher.isNil()) {
        return;
    }
    dispatcher(event, args...);
}

void Desktop::paintEvent(QPaintEvent* const event)
{
    dispatch(_lua, "RENDER", LuaPainter(this));
}

void Desktop::wheelEvent(QWheelEvent* const event)
{
    dispatch(_lua, "MOUSEWHEEL", event->delta());
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

void Desktop::mousePressEvent(QMouseEvent* const event)
{
    dispatch(_lua, "MOUSEPRESS", buttonName(event->button()));
}

void Desktop::mouseReleaseEvent(QMouseEvent* const event)
{
    dispatch(_lua, "MOUSERELEASE", buttonName(event->button()));
}

void Desktop::mouseMoveEvent(QMouseEvent* const event)
{
    dispatch(_lua, "MOUSEMOVE", event->x(), event->y());
}

// vim: set ts=4 sw=4 :
