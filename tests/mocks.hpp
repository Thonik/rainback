#include <QObject>

class StringSink : public QObject {
    Q_OBJECT

public:
    QList<QString> strings;

public slots:
    void add(const QString& str);
};
