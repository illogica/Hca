#include "listworldsworker.h"
#include "protocol.h"
#include <QJsonArray>
#include <QJsonObject>

ListWorldsWorker::ListWorldsWorker(){}

void ListWorldsWorker::doWork(HcaThread *t)
{
    emit(t->setThreadStatus(m_id, false)); //this must always be the first
    m_id = t->id();
    m_db = t->db();

    QJsonArray worldsList;
    QSqlQuery query("SELECT * FROM worlds", m_db);
    while(query.next()){
        QJsonObject worldObj;
        qint32 worldId = query.value(0).toInt();
        worldObj[WORLD_ID]=worldId;
        worldObj[WORLD_NAME]=query.value(1).toString();
        worldObj[DESCRIPTION]=query.value(2).toString();

        //get the world quantity of rooms
        QSqlQuery sizeQuery(m_db);
        sizeQuery.prepare("SELECT COUNT(*) FROM rooms WHERE rooms.world = :id");
        sizeQuery.bindValue(":id", worldId);
        if(!sizeQuery.exec())
            emit dbError(sizeQuery.lastError().text());
        qint32 worldSize = 0;
        if(sizeQuery.next()){
            worldSize = sizeQuery.value(0).toInt();
        }

        worldObj[WORLD_SIZE]=worldSize;
        worldsList.append(worldObj);
    }

    QJsonObject response;
    response[REQUEST] = LIST_WORLDS;
    response[WORLDS] = worldsList;
    QJsonDocument doc;
    doc.setObject(response);

    qWarning() << "Emitting listWorlds from " << QThread::currentThreadId();
    emit listWorldsResult(doc.toJson(QJsonDocument::Compact), socket);
    emit(t->setThreadStatus(m_id, true)); //this must always be the last
}
