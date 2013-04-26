#include "Rainback.hpp"

#include <functional>
#include <QTextStream>
#include <QApplication>
#include <QPushButton>

#include <lua-cxx/LuaValue.hpp>
#include <lua-cxx/loaders.hpp>
#include <lua-cxx/userdata.hpp>
#include <lua-cxx/QObjectObserver.hpp>

#include "LuaPainter.hpp"
#include "LuaFont.hpp"

namespace lua {

template<>
struct UserdataType<QWidget>
{
    constexpr static const char* name = "QWidget";

    static void initialize(LuaStack& stack, QWidget& widget)
    {
        auto userdata = stack.save();
        lua::push(stack, userdata);
        lua::userdata::qobject(stack, widget);

        lua::push(stack, lua::value::table);
        auto methods = stack.save();

        methods["resize"] = std::function<void(QWidget&, const int, const int)>(
            [](QWidget& widget, const int w, const int h) {
                widget.resize(w, h);
            }
        );

        auto worker = stack.lua()(""
            "return function(userdata, methods)\n"
            "    local mt = getmetatable(userdata);\n"
            ""
            "    local defaultIndex = mt.__index;\n"
            "    function mt.__index(self, key)\n"
            "        return methods[key] or defaultIndex(self, key);"
            "    end;\n"
            "end;\n"
        );
        worker(userdata, methods);
    }
};

} // namespace lua

std::shared_ptr<LuaFont> newFont(LuaStack& stack)
{
    switch (stack.size()) {
    case 0:
        return std::make_shared<LuaFont>();
    case 1:
        return std::make_shared<LuaFont>(stack.as<QString>(1));
    default:
        return std::make_shared<LuaFont>(
            stack.as<QString>(1),
            stack.as<int>(2)
        );
    }
}

namespace rainback {

void Rainback::assertWidget()
{
    if (!_widget) {
        throw LuaException("This function must not be used when no widget is available");
    }
}

Rainback::Rainback(Lua& lua) :
    _widget(nullptr),
    _lua(lua)
{
    _lua["Rainback"] = lua::value::table;

    _lua["Rainback"]["AddModuleDirectory"] =
    std::function<void(const std::string&, const std::string&)>(
        [this](const std::string& root, const std::string& prefix) {
            auto loader = std::make_shared<DirectoryModuleLoader>();
            loaders.push_back(loader);
            loader->setRoot(QDir(root.c_str()));
            loader->setPrefix(prefix);
            _lua.addModuleLoader(loader.get());
        }
    );

    _lua["Rainback"]["LoadDirectory"] = std::function<void(const std::string&, const bool)>(
        [this](const std::string& path, const bool recurse) {
            lua::load_dir(_lua, QDir(path.c_str()), recurse);
        }
    );

    _lua["Rainback"]["Font"] = newFont;

    _lua["Rainback"]["Button"] = lua::LuaCallable([this](LuaStack& stack) {
            assertWidget();
            stack.clear();

            auto btn = new QPushButton(_widget);
            lua::push<QWidget*>(stack, btn, true);
            new lua::QObjectObserver(btn, stack.as<LuaUserdata*>(-1));

            btn->setGeometry(50, 50, 150, 50);
            btn->show();
        }
    );

    _lua["Rainback" ]["ScreenWidth"] = std::function<int()>([this]() {
        assertWidget();
        return _widget->width();
    });

    _lua["Rainback" ]["ScreenHeight"] = std::function<int()>([this]() {
        assertWidget();
        return _widget->height();
    });

    _lua["Rainback"]["GetTime"] = std::function<double()>([this]() {
        return elapsed.elapsed() / 1000;
    });

    _lua["Rainback"]["StartTick"] =
    std::function<std::function<void()>(const int)>([this](const int framerate) {
        timer.start(1000 / framerate);
        return [this]() {
            timer.stop();
        };
    });

    _lua["Rainback"]["GetCursorPosition"] =
    std::function<void(LuaStack&)>([this](LuaStack& stack) {
        assertWidget();
        QPoint pos(_widget->mapFromGlobal(QCursor::pos()));
        stack.clear();
        lua::push(stack, pos.x());
        lua::push(stack, pos.y());
    });

    _lua["Rainback"]["Update"] = std::function<void()>([this]() {
        assertWidget();
        _widget->update();
    });

    _lua["Rainback"]["SetBackgroundColor"] = std::function<void(const int, const int, const int)>(
        [this](const int r, const int g, const int b) {
            assertWidget();
            _widget->setAutoFillBackground(true);
            QPalette p(_widget->palette());
            p.setColor(QPalette::Background, QColor(r, g, b));
            _widget->setPalette(p);
        }
    );

    _lua["Rainback"]["FileExists"] = std::function<bool(const QString&)>(
        [this](const QString& fileName) {
            return QFile(fileName).exists();
        }
    );

    _lua["Rainback"]["ReadFile"] = std::function<QString(const QString&)>(
        [this](const QString& fileName) {
            QFile file(fileName);
            openFile(file, QIODevice::ReadOnly, true);
            // Using QTextStream here breaks for Unicode strings
            return QString(file.readAll());
        }
    );

    _lua["Rainback"]["ReadCompressedFile"] = std::function<QString(const QString&)>(
        [this](const QString& fileName) {
            QFile file(fileName);
            openFile(file, QIODevice::ReadOnly, true);
            return QString(qUncompress(file.readAll()));
        }
    );

    _lua["Rainback"]["WriteFile"] = std::function<void(const QString&, const QString&)>(
        [this](const QString& fileName, const QString& data) {
            QFile file(fileName);
            openFile(file, QIODevice::WriteOnly | QIODevice::Truncate, false);
            QTextStream(&file) << data;
        }
    );

    _lua["Rainback"]["WriteCompressedFile"] = std::function<void(const QString&, const QString&)>(
        [this](const QString& fileName, const QString& data) {
            QFile file(fileName);
            openFile(file, QIODevice::WriteOnly | QIODevice::Truncate, false);
            file.write(qCompress(data.toUtf8()));
        }
    );

    elapsed.start();

    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(qApp, SIGNAL(lastWindowClosed()), this, SLOT(close()));
}

void Rainback::openFile(QFile& file, const QIODevice::OpenMode& flags, const bool checkExists)
{
    if (checkExists && !file.exists()) {
        throw LuaException(
            (QString("The specified file '") + file.fileName() + "' does not exist").toStdString()
        );
    }
    if (!file.open(flags)) {
        throw LuaException(
            (QString("The specified file '") + file.fileName() + "' failed to open: " + file.error()).toStdString()
        );
    }
}

void Rainback::timeout()
{
    rainback::dispatch(_lua, "UPDATE");
}

void Rainback::close()
{
    rainback::dispatch(_lua, "CLOSE");
}

} // namespace rainback

// vim: set ts=4 sw=4 :
