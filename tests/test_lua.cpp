#include "Rainback.hpp"

#include <lua-cxx/LuaEnvironment.hpp>
#include <lua-cxx/LuaValue.hpp>
#include <QFile>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    Lua lua;
    rainback::Rainback rainback(lua);

    std::string srcdir(getenv("srcdir"));
    QFile testRunner(QString(srcdir.c_str()) + "/test_lua.lua");
    return (bool)lua(testRunner) ? 0 : 1;
}
