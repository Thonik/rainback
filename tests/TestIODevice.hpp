#include <QIODevice>

class TestIODevice : public QIODevice
{
    QByteArray readStore;
    QByteArray writeStore;
protected:
    qint64 readData(char* data, qint64 maxSize);
    qint64 writeData(const char* data, qint64 maxSize);
public:
    void fakeWrite(const QByteArray& data);

    bool isSequential() const
    {
        return true;
    }

    qint64 bytesAvailable() const
    {
        return readStore.size() + QIODevice::bytesAvailable();
    }

    bool canReadLine() const
    {
        return readStore.contains('\n') || QIODevice::canReadLine();
    }
};
