#include "init.hpp"
#include "TestIODevice.hpp"

#include "protocol/Human.hpp"
#include "protocol/Pascal.hpp"
#include <QBuffer>
#include <QDataStream>

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

void writeRawPascal(QByteArray& array, const QString& line)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    protocol::Pascal writer;
    writer.listen(&buffer);
    writer.write(line);

    array.append(buffer.data());
}

void writePascal(TestIODevice& device, const QString& line)
{
    QByteArray sink;
    writeRawPascal(sink, line);

    device.fakeWrite(sink);
}

BOOST_AUTO_TEST_CASE(checkPascalProtocol)
{
    TestIODevice io;
    io.open(QIODevice::ReadWrite);

    protocol::Pascal ptl;

    StringSink dump;
    QObject::connect(&ptl, SIGNAL(lineRead(const QString&)), &dump, SLOT(add(const QString&)));

    ptl.listen(&io);

    BOOST_TEST_CHECKPOINT("Protocol handles basic pascal strings");
    writePascal(io, "J");
    BOOST_CHECK_EQUAL(dump.strings.size(), 1);
    BOOST_CHECK_EQUAL(dump.strings[0].toStdString(), "J");

    dump.strings.clear();
    BOOST_REQUIRE(dump.strings.isEmpty());

    BOOST_TEST_CHECKPOINT("Protocol handles segmented data");
    QByteArray segmented;
    writeRawPascal(segmented, "A");
    io.fakeWrite(segmented.left(1));
    io.fakeWrite(segmented.mid(1));

    BOOST_CHECK_EQUAL(dump.strings.size(), 1);
    BOOST_CHECK_EQUAL(dump.strings[0].toStdString(), "A");

    segmented.clear();
    dump.strings.clear();

    BOOST_TEST_CHECKPOINT("Protocol handles segmented data after count");
    writeRawPascal(segmented, "B");
    io.fakeWrite(segmented.left(5));
    io.fakeWrite(segmented.mid(5));

    BOOST_CHECK_EQUAL(dump.strings.size(), 1);
    BOOST_CHECK_EQUAL(dump.strings[0].toStdString(), "B");

    segmented.clear();
    dump.strings.clear();

    BOOST_TEST_CHECKPOINT("Protocol can split a single packet containing multiple commands");
    writeRawPascal(segmented, "D");
    writeRawPascal(segmented, "E");
    io.fakeWrite(segmented);
    BOOST_CHECK_EQUAL(dump.strings.size(), 2);
    BOOST_CHECK_EQUAL(dump.strings[0].toStdString(), "D");
    BOOST_CHECK_EQUAL(dump.strings[1].toStdString(), "E");
}

// vim: set ts=4 sw=4 :
