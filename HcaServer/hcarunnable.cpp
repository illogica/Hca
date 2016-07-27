#include "hcarunnable.h"

HcaRunnable::HcaRunnable()
{

}

void HcaRunnable::run()
{
    if(!connect()) return;
    m_el = new QEventLoop(this);
    m_el->exec();
}

bool HcaRunnable::connect()
{
    threadId = QString::number((qintptr)QThread::currentThreadId());
    db = QSqlDatabase::addDatabase("QPSQL", threadId);
    db.setHostName("127.0.0.1");
    db.setDatabaseName("hca");
    db.setUserName("hca");
    //db.setPassword("");
    if(!db.open()){
        qWarning() << "Error opening database: " << db.lastError().text();
        emit dbError(db.lastError().text());
        return false;
    } else {
        qWarning() << "Database open";
        return true;
    }
}

bool HcaRunnable::findUser()
{
    return false;
}

void HcaRunnable::createUser()
{

}

void HcaRunnable::login()
{
    qWarning() << "running on thread " << QThread::currentThreadId();
}
