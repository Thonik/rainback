#include "TestIODevice.hpp"

#include <iostream>

void TestIODevice::fakeWrite(const QByteArray& data)
{
    readStore.append(data);
    emit readyRead();
}

qint64 TestIODevice::readData(char* data, qint64 maxSize)
{
    auto readData = readStore.left(maxSize);
    //std::cerr << "Read " << readData.size() << " byte(s) from mock IO device, requested " << maxSize << std::endl;
    qstrcpy(data, readData.constData());
    if (readStore.size() == readData.size()) {
        readStore.clear();
    } else {
        readStore = readStore.right(readStore.size() - readData.size());
    }
    return readData.size();
}

qint64 TestIODevice::writeData(const char* data, qint64 maxSize)
{
    writeStore.append(data, maxSize);
    return maxSize;
}
