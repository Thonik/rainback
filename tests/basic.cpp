#include "init.hpp"
#include "TestIODevice.hpp"

#include "protocol/Human.hpp"
#include <QBuffer>

#include <QCoreApplication>

#include <QObject>

using namespace rainback;

BOOST_AUTO_TEST_CASE(checkHumanProtocol)
{
    TestIODevice io;
    io.open(QIODevice::ReadWrite);

    protocol::Human ptl;

    StringSink dump;

    QObject::connect(&ptl, SIGNAL(lineRead(const QString&)), &dump, SLOT(add(const QString&)));

    ptl.listen(&io);

    BOOST_TEST_CHECKPOINT("Protocol receives commands");
    io.fakeWrite("A\n");
    BOOST_CHECK_EQUAL(dump.strings.size(), 1);
    BOOST_CHECK_EQUAL(dump.strings[0].toStdString(), "A");

    dump.strings.clear();
    BOOST_REQUIRE(dump.strings.isEmpty());

    BOOST_TEST_CHECKPOINT("Protocol removes carriage return");
    io.fakeWrite("B\r\n");
    BOOST_CHECK_EQUAL(dump.strings.size(), 1);
    BOOST_CHECK_EQUAL(dump.strings[0].toStdString(), "B");

    dump.strings.clear();

    BOOST_TEST_CHECKPOINT("Protocol handles segmented data");
    io.fakeWrite("B");
    io.fakeWrite("C");
    io.fakeWrite("\n");
    BOOST_CHECK_EQUAL(dump.strings.size(), 1);
    BOOST_CHECK_EQUAL(dump.strings[0].toStdString(), "BC");

    dump.strings.clear();

    BOOST_TEST_CHECKPOINT("Protocol can split a single packet containing multiple commands");
    io.fakeWrite("D\nE\n");
    BOOST_CHECK_EQUAL(dump.strings.size(), 2);
    BOOST_CHECK_EQUAL(dump.strings[0].toStdString(), "D");
    BOOST_CHECK_EQUAL(dump.strings[1].toStdString(), "E");
}

// vim: set ts=4 sw=4 :
