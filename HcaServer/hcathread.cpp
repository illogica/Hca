#include "hcathread.h"

HcaThread::HcaThread(const QString &name)
{
    if (!QSqlDatabase::drivers().contains("QPSQL")){
        qWarning() << "Unable to load database, PSQL driver missing";
        return;
    }

    db = QSqlDatabase::addDatabase("QPSQL", name);
    db.setHostName("127.0.0.1");
    db.setDatabaseName("hca");
    db.setUserName("hca");
    //db.setPassword("");
    if(!db.open()){
        qWarning() << db.lastError().text();
    } else {
        qWarning() << "This FUCKING WORKS";
    }
}
