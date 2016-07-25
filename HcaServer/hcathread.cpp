#include "hcathread.h"
#include "hcaserver.h"

HcaThread::HcaThread(int id, HcaServer* srv)
{
    m_id = id;
    server = srv;
}

void HcaThread::run()
{
    if (!QSqlDatabase::drivers().contains("QPSQL")){
        qWarning() << "Unable to load database, PSQL driver missing";
        return;
    }
    qWarning() << "Thread: " << QThread::currentThreadId();
    connect(server, &HcaServer::testThread, this, &HcaThread::testText);

    db = QSqlDatabase::addDatabase("QPSQL", QString(m_id));
    db.setHostName("127.0.0.1");
    db.setDatabaseName("hca");
    db.setUserName("hca");
    //db.setPassword("");
    if(!db.open()){
        qWarning() << db.lastError().text();
    } else {
        emit initialized(m_id);
        exec();
    }
}

void HcaThread::testText(){
    qWarning() << "TestText from " << QThread::currentThreadId();
}
