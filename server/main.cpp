#include "Bootstrapper.hpp"

#include <QCoreApplication>
#include <QMainWindow>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    Bootstrapper bootstrapper;

    return app.exec();
}

// vim: set ts=4 sw=4 :
