#include "Rainback.hpp"

#include <iostream>

#include <functional>
#include <QTextStream>
#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QRegExp>
#include <QFileSystemWatcher>
#include <QTcpSocket>
#include <QProcess>

#include <lua-cxx/LuaValue.hpp>
#include <lua-cxx/loaders.hpp>
#include <lua-cxx/userdata.hpp>
#include <lua-cxx/QObjectObserver.hpp>

#include "proxy/Painter.hpp"
#include "proxy/Font.hpp"
#include "proxy/TCPServer.hpp"
#include "proxy/ByteArray.hpp"
#include "protocol/Human.hpp"
#include "protocol/Pascal.hpp"

#include "proxy/QObject.hpp"
#include "proxy/Socket.hpp"
#include "proxy/TCPServer.hpp"
#include "proxy/QWidget.hpp"
#include "proxy/AbstractLine.hpp"

using namespace rainback;

std::shared_ptr<proxy::Font> newFont(LuaStack& stack)
{
    switch (stack.size()) {
    case 0:
        return std::make_shared<proxy::Font>();
    case 1:
        return std::make_shared<proxy::Font>(stack.as<QString>(1));
    default:
        return std::make_shared<proxy::Font>(
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
	if (_lua["unpack"].isNil()) {
		_lua["unpack"] = _lua["table"]["unpack"];
	}

    _lua["Rainback"] = lua::value::table;

    _lua["error"] = lua::LuaCallable([](LuaStack& stack) {
        stack.error(stack.as<std::string>(-1));
    });

    _lua["Rainback"]["globals"] = lua::value::table;

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
            lua::push<QWidget*>(stack, new QPushButton(_widget), true);
        }
    );

    _lua["Rainback"]["LineEdit"] = lua::LuaCallable([this](LuaStack& stack) {
            assertWidget();
            stack.clear();
            lua::push<QWidget*>(stack, new QLineEdit(_widget), true);
        }
    );

    _lua["Rainback"]["TextEdit"] = lua::LuaCallable([this](LuaStack& stack) {
            assertWidget();
            stack.clear();
            lua::push<QWidget*>(stack, new QTextEdit(_widget), true);
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

    _lua["Rainback"]["GetTime"] = std::function<long()>([this]() {
        return elapsed.elapsed();
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

    _lua["Rainback"]["Close"] = std::function<void()>([this]() {
        assertWidget();
        qApp->closeAllWindows();
    });

    _lua["Rainback"]["Exit"] = std::function<void()>([this]() {
        QCoreApplication::exit();
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

    _lua["Rainback"]["CreateDirectory"] = std::function<bool(const QString&)>(
        [this](const QString& dirName) {
            return QDir::current().mkpath(dirName);
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

    _lua["Rainback"]["WatchFile"] = std::function<void(LuaStack&)>(
        [this](LuaStack& stack) {
            auto watcher = new QFileSystemWatcher(this);

            for (int i=1; i <= stack.size(); ++i) {
                watcher->addPath(stack.as<QString>(i));
            }
            stack.clear();

            lua::push<QObject*>(stack, watcher, true);
            proxy::observeToDestroy(stack, *watcher, true);
        }
    );

    _lua["string"]["grep"] = lua::LuaCallable([](LuaStack& stack) {
        auto str = stack.as<QString>(1);
        QRegExp re(stack.as<QString>(2));
        stack.clear();

        if (re.indexIn(str) == -1) {
            return;
        }

        for (int i = 0; i <= re.captureCount(); ++i) {
            lua::push(stack, re.cap(i));
        }
    });

    _lua["Rainback"]["Exec"] = lua::LuaCallable(
        [this](LuaStack& stack) {
            QProcess process;

            QString progName = stack.as<QString>(1);
            stack.shift();

            QStringList args;
            while (!stack.empty()) {
                args << stack.as<QString>(1);
                stack.shift();
            }
            stack.clear();

            process.start(progName, args);
            process.waitForFinished(3000);

            lua::push(stack, std::make_shared<QByteArray>(process.readAllStandardOutput()));
        }
    );

    _lua["Rainback"]["Network"] = lua::value::table;

    _lua["Rainback"]["Network"]["serveTCP"] = lua::LuaCallable(
        [this](LuaStack& stack) {
            if (stack.size() < 1) {
                throw LuaException("serverTCP requires 1 argument, but the stack has none");
            }

            auto port = stack.as<int>(1);
            proxy::validatePort(port);
            stack.clear();

            auto server = new proxy::TCPServer;
            auto socketServer = new QTcpServer(server);
            server->setServer(socketServer);

            lua::push<QObject*>(stack, server, true);
            proxy::observeToDestroy(stack, *server, true);

            socketServer->listen(QHostAddress::Any, port);
        }
    );

    _lua["Rainback"]["Network"]["connectTCP"] = lua::LuaCallable(
        [this](LuaStack& stack) {
            if (stack.size() < 2) {
                std::stringstream str;
                str << "connectTCP requires 2 arguments, but was given only " << stack.size();
                throw LuaException(str.str());
            }
            auto hostname = stack.as<QString>(1);
            auto port = stack.as<int>(2);
            proxy::validatePort(port);
            stack.clear();

            QAbstractSocket* socket = new QTcpSocket(this);
            lua::push<QAbstractSocket*>(stack, socket, true);

            socket->connectToHost(hostname, static_cast<quint16>(port));

            return socket;
        }
    );

    _lua["Rainback"]["Network"]["pascalProtocol"] = std::function<std::shared_ptr<protocol::AbstractLine>(LuaStack& stack)>(
        [this](LuaStack& stack) {
            auto ptl = std::make_shared<protocol::Pascal>();

            if (stack.size() > 0) {
                ptl->listen(stack.as<QAbstractSocket*>());
            }

            return ptl;
        }
    );

    _lua["Rainback"]["Network"]["humanProtocol"] = std::function<std::shared_ptr<protocol::AbstractLine>(LuaStack& stack)>(
        [this](LuaStack& stack) {
            std::shared_ptr<protocol::AbstractLine> ptl(std::make_shared<protocol::Human>());

            if (stack.size() > 0) {
                ptl->listen(stack.as<QAbstractSocket*>());
            }

            return ptl;
        }
    );

    lua::qvariantPusher(QVariant::Size, [](LuaStack& stack, const QVariant& source) {
        lua::push(stack, lua::value::table);

        auto size = source.toSize();
        stack.set("width", size.width());
        stack.set("height", size.height());
    });

    elapsed.start();

    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(close()));
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
