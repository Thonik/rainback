#include "Desktop.hpp"

Desktop::Desktop(Lua& lua) :
    _lua(lua)
{
    QPalette p(palette());
    p.setColor(QPalette::Background, QColor(255, 255, 221));
    _desktop.setAutoFillBackground(true);
    _desktop.setPalette(p);
}

void Desktop::paintEvent(QPaintEvent* event)
{
    auto dispatcher = lua["Rainback"]["DispatchEvent"];
    if (dispatcher.isNil()) {
        return;
    }
    dispatcher("RENDER", LuaPainter(this));
}

// vim: set ts=4 sw=4 :
