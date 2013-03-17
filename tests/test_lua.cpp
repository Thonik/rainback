#include <lua-cxx/LuaEnvironment.hpp>
#include <lua-cxx/LuaValue.hpp>
#include <lua-cxx/DirectoryModuleLoader.hpp>
#include <lua-cxx/loaders.hpp>
#include <QFile>
#include <QDir>

#include <cstdlib>

int main(int argc, char* argv[])
{
    Lua lua;

    std::string srcdir(getenv("srcdir"));

    DirectoryModuleLoader fritomodLoader;
    fritomodLoader.setRoot(QDir(QDir::home().filePath("src/fritomod/fritomod")));
    fritomodLoader.setPrefix("fritomod/");
    lua.addModuleLoader(&fritomodLoader);

    DirectoryModuleLoader rainbackLoader;
    rainbackLoader.setRoot(QDir(QString(srcdir.c_str()) + "/../src/rainback"));
    rainbackLoader.setPrefix("rainback/");
    lua.addModuleLoader(&rainbackLoader);

    lua::load_file(lua, srcdir + "/global.lua");
    lua::load_dir(lua, QDir(QString(srcdir.c_str()) + "/rainback"), true);

    QFile testRunner(QString(srcdir.c_str()) + "/tests.lua");
    return (bool)lua(testRunner) ? 0 : 1;
}
