#include "disconnectworker.h"
#include "protocol.h"

void DisconnectWorker::doWork(HcaThread *t)
{
    m_id = t->id();
    m_db = t->db();
    emit(t->setThreadStatus(m_id, false)); //this must always be the first

    QSqlQuery updQuery(m_db);
    updQuery.prepare("UPDATE clients SET status = :st WHERE uuid = :uuid");
    updQuery.bindValue(":st", OFFLINE);
    updQuery.bindValue(":uuid", uuid);
    if(!updQuery.exec()){
        qWarning() << "Error in " << updQuery.lastQuery();
        emit dbError(updQuery.lastError().text());
    };
    //user found and updated

    qWarning() << "Emitting disconnect from " << QThread::currentThreadId();
    emit disconnectResult();
    emit(t->setThreadStatus(m_id, true)); //this must always be the last
}
