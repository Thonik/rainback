#ifndef RAINBACK_PROXY_QWIDGET_HEADER
#define RAINBACK_PROXY_QWIDGET_HEADER

#include "proxy/QObject.hpp"

namespace lua {

template<>
struct UserdataType<QWidget>
{
    constexpr static const char* name = "QWidget";

    static void initialize(LuaStack& stack, QWidget& widget)
    {
        lua::push(stack, lua::value::table);
        auto methods = stack.saveAndPop();

        methods["resize"] = std::function<void(QWidget&, const int, const int)>(
            [](QWidget& widget, const int w, const int h) {
                widget.resize(w, h);
            }
        );

        methods["setGeometry"] = std::function<void(QWidget&, const int, const int, const int, const int)>(
            [](QWidget& widget, const int x, const int y, const int w, const int h) {
                widget.setGeometry(x, y, w, h);
            }
        );

        rainback::proxy::wrapQObject(stack, widget, methods);
        rainback::proxy::observeToDestroy(stack, widget, true);
    }
};

} // namespace lua

#endif // RAINBACK_PROXY_QWIDGET_HEADER

// vim: set ts=4 sw=4 :
