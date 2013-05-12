#ifndef RAINBACK_BOOTSTRAPPER_HEADER
#define RAINBACK_BOOTSTRAPPER_HEADER

#include "Rainback.hpp"
#include <lua-cxx/LuaEnvironment.hpp>
#include "Server.hpp"

class Bootstrapper
{
    Lua _lua;
    rainback::Rainback _rainback;

    Server server;
public:
    Bootstrapper();
};

#endif // RAINBACK_BOOTSTRAPPER_HEADER


// vim: set ts=4 sw=4 :
