#ifndef RAINBACK_RAINBACK_HEADER
#define RAINBACK_RAINBACK_HEADER

#include <vector>
#include <memory>

#include <lua-cxx/DirectoryModuleLoader.hpp>
#include <lua-cxx/LuaValue.hpp>

#include <QWidget>
#include <QElapsedTimer>
#include <QTimer>

namespace rainback {

class Rainback : public QObject
{
    Q_OBJECT
    QWidget* _widget;

    Lua& _lua;

    QElapsedTimer elapsed;
    QTimer timer;

    DirectoryModuleLoader fritomodLoader;
    DirectoryModuleLoader wowLoader;
    DirectoryModuleLoader rainbackLoader;
    std::vector<std::shared_ptr<ModuleLoader>> loaders;

    void assertWidget();

public:
    Rainback(Lua& lua);

    QWidget* widget()
    {
        return _widget;
    }

    void setWidget(QWidget* const widget)
    {
        _widget = widget;
    }

    Lua& lua() const
    {
        return _lua;
    }

private slots:
    void timeout();
};

template <typename... Args>
void dispatch(Lua& lua, std::string event, Args&&... args)
{
    auto dispatcher = lua["Rainback"]["DispatchEvent"];
    if (dispatcher.isNil()) {
        return;
    }
    dispatcher(event, args...);
}

} // namespace rainback

#endif // RAINBACK_RAINBACK_HEADER


// vim: set ts=4 sw=4 :
