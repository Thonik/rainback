#include "Bootstrapper.hpp"

#include <functional>
#include <iostream>

#include <lua-cxx/LuaValue.hpp>
#include <lua-cxx/loaders.hpp>
#include <lua-cxx/userdata.hpp>

#include "LuaPainter.hpp"
#include "LuaFont.hpp"

#include <QCoreApplication>

Bootstrapper::Bootstrapper() :
    _lua(),
    _desktop(_lua),
    _rainback(_lua)
{
    _rainback.setWidget(&_desktop);

	const QString buildDir(TOP_BUILDDIR);
	const QString srcDir(TOP_SRCDIR);

	QFile buildProps(buildDir + "/settings.lua");
	QFile srcProps(srcDir + "/settings.lua");

    if (srcProps.exists()) {
        _lua(srcProps);
    }
    if (buildProps.exists()) {
        _lua(buildProps);
    }
	if (!srcProps.exists() && !buildProps.exists()) {
		std::cerr << "settings.lua must be defined in your top build or source directory" << std::endl;
		std::cerr << "Look at demo/init.lua for guidance on what globals need to be defined" << std::endl;
		std::cerr << "defined within that file. Build settings will override source settings." << std::endl;
		QCoreApplication::exit(1);
        return;
	}

	_lua["Rainback"]["globals"]["srcdir"] = srcDir;
	_lua["Rainback"]["globals"]["buildDir"] = buildDir;

	QFile initFile(srcDir + "/demo/init.lua");
    _lua(initFile);
}

QWidget& Bootstrapper::mainWidget()
{
    return _desktop;
}

// vim: set ts=4 sw=4 :
