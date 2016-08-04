#include "templateworker.h"

void TemplateWorker::doWork(HcaThread *t)
{
    m_id = t->id();
    DbManager* dbm = t->dbManager();
    Q_UNUSED(dbm)
    emit(t->setThreadStatus(m_id, false)); //this must always be the first

    QJsonObject response;
    response[REQUEST] = 0; //CHANGE THIS
    QJsonDocument doc;
    doc.setObject(response);


    emit templateWorkerResult(doc.toJson(QJsonDocument::Compact), socket);
    emit(t->setThreadStatus(m_id, true)); //this must always be the last
}
