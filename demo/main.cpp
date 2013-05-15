#include "Bootstrapper.hpp"
#include "util.hpp"

#include <QApplication>
#include <QMainWindow>

int runClient(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMainWindow gui;
    gui.setWindowTitle("Rainback v1.0");
    gui.resize(1280, 695);

    Bootstrapper bootstrapper;
    gui.setCentralWidget(&bootstrapper.mainWidget());

    gui.show();

    return app.exec();
}

int runServer(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    Lua _lua;
    rainback::Rainback rainback(_lua);

	const QString buildDir(TOP_BUILDDIR);
	const QString srcDir(TOP_SRCDIR);

	_lua["Rainback"]["globals"]["srcdir"] = srcDir;
	_lua["Rainback"]["globals"]["buildDir"] = buildDir;
	_lua["Rainback"]["globals"]["profile"] = "server";

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
		return 1;
	}

	QFile initFile(srcDir + "/demo/init.lua");
	_lua(initFile);

    return app.exec();
}

int main(int argc, char* argv[])
{
    std::string profile;
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "server") {
            profile = "server";
        }
    }

    if (profile == "server") {
        return runServer(argc, argv);
    }
    return runClient(argc, argv);
}

// vim: set ts=4 sw=4 :
