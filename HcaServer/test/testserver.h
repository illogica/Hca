#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <QtTest/QtTest>
#include <QObject>

class TestServer : public QObject
{
    Q_OBJECT
public:
    explicit TestServer(QObject *parent = 0);

signals:

public slots:

private slots:
    void toUpper();
};


#endif // TESTSERVER_H
