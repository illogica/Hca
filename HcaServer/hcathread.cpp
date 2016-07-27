#include "hcathread.h"
#include "hcaserver.h"

void HcaThread::run()
{
    m_id = QString::number((qintptr)QThread::currentThreadId());
    if (!QSqlDatabase::drivers().contains("QPSQL")){
        qWarning() << "Unable to load database, PSQL driver missing";
        return;
    }
    qWarning() << "Thread: " << QThread::currentThreadId();

    m_db = QSqlDatabase::addDatabase("QPSQL", QString(m_id));
    m_db.setHostName("127.0.0.1");
    m_db.setDatabaseName("hca");
    m_db.setUserName("hca");
    //db.setPassword("");
    if(!m_db.open()){
        qWarning() << "Error opening database: " << m_db.lastError().text();
    } else {
        emit initialized(this);
        exec();
    }
}

QSqlDatabase HcaThread::db() const {return m_db;}

QString HcaThread::id() const {return m_id;}

