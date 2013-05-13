#define BOOST_TEST_MODULE rainback
#include "init.hpp"

#include <QCoreApplication>

struct QApplicationFixture
{
    int argc;
    char name[100];
    char* argv[1];

    QCoreApplication* app;
public:
    QApplicationFixture() :
        argc(1)
    {
        strcpy(name, "rainback");
        argv[0] = name;
        app = new QCoreApplication(argc, argv);
    }

    ~QApplicationFixture()
    {
        delete app;
    }
};

BOOST_GLOBAL_FIXTURE(QApplicationFixture);

// vim: set ts=4 sw=4 :
