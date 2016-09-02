#include "testserver.h"

TestServer::TestServer(QObject *parent) : QObject(parent)
{

}

void TestServer::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}
