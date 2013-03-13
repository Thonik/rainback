// To build this file from scratch, use something like the following:
// g++ -lQtCore -lQtGui -I/usr/include/QtCore -I/usr/include/QtGui main.cpp

#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QPushButton>
#include <QSize>

void centerApp(QMainWindow& window)
{
    QSize appSize(800, 600);
    window.resize(appSize);

    QDesktopWidget* desktop = QApplication::desktop();

    int desktopArea = desktop->width() * desktop->height();
    int appArea = appSize.width() * appSize.height();
    if (((float)appArea / (float)desktopArea) > 0.75f) {
        // Just maximize it if the desktop isn't significantly
        // bigger than our app's area.
        window.showMaximized();
    } else {
        // Center the app on the primary monitor.
        QPoint windowLocation = desktop->screenGeometry(desktop->primaryScreen()).center();
        windowLocation.setX(windowLocation.x() - appSize.width() / 2);
        windowLocation.setY(windowLocation.y() - appSize.height() / 2);
        window.move(windowLocation);
        window.show();
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow gui;
    gui.setWindowTitle("rainback");
    centerApp(gui);

    QPushButton quit("Quit", &gui);
    quit.setGeometry(10, 40, 180, 40);
    QObject::connect(&quit, SIGNAL(clicked()), &app, SLOT(quit()));

    gui.setCentralWidget(&quit);

    return app.exec();
}

// vim: set ts=4 sw=4 :
