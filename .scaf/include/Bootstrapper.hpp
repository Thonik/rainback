#ifndef RAINBACK_BOOTSTRAPPER_HEADER
#define RAINBACK_BOOTSTRAPPER_HEADER

#include <QWidget>

class Bootstrapper
{
    QWidget _mainWidget;
public:
    Bootstrapper();

    QWidget& mainWidget();
};

#endif // RAINBACK_BOOTSTRAPPER_HEADER


// vim: set ts=4 sw=4 :
