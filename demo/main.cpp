#include "Bootstrapper.hpp"
#include "util.hpp"

#include <QApplication>
#include <QMainWindow>

int main(int argc, char* argv[])
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

// vim: set ts=4 sw=4 :
