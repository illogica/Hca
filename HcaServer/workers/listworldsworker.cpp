#include "listworldsworker.h"
#include "protocol.h"
#include <QJsonArray>
#include <QJsonObject>

ListWorldsWorker::ListWorldsWorker(){}

void ListWorldsWorker::doWork(HcaThread *t)
{
    m_id = t->id();
    DbManager* dbm = t->dbManager();
    emit(t->setThreadStatus(m_id, false)); //this must always be the first

    QJsonArray worldsArray;

    QList<World*> worlds;
    dbm->listWorlds(worlds);
    for(World *w:worlds){
        worldsArray.append(w->toJsonObject());
    }

    QJsonObject response;
    response[REQUEST] = LIST_WORLDS;
    response[WORLDS] = worldsArray;
    QJsonDocument doc;
    doc.setObject(response);

    qWarning() << "Emitting listWorlds from " << QThread::currentThreadId();
    emit listWorldsResult(doc.toJson(QJsonDocument::Compact), socket);
    emit(t->setThreadStatus(m_id, true)); //this must always be the last
}
