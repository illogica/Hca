#include "listroomsworker.h"
#include "protocol.h"

ListRoomsWorker::ListRoomsWorker(){}

void ListRoomsWorker::doWork(HcaThread *t)
{
    m_id = t->id();
    m_db = t->db();
    emit(t->setThreadStatus(m_id, false)); //this must always be the first

    QJsonArray roomsList;
    QSqlQuery query(m_db);
    query.prepare("SELECT rooms.id, rooms.name, rooms.description FROM worlds, rooms WHERE rooms.worldid = :id");
    query.bindValue(":id", worldId);
    while(query.next()){
        QJsonObject roomObj;
        qint32 roomId = query.value(0).toInt();
        roomObj[ROOM_ID]=roomId;
        roomObj[ROOM_NAME]=query.value(1).toString();
        roomObj[DESCRIPTION]=query.value(2).toString();

        //get the quantity of users per room
        QSqlQuery sizeQuery(m_db);
        sizeQuery.prepare("SELECT COUNT (*) FROM roomclients where roomid = :id");
        sizeQuery.bindValue(":id", roomId);
        if(!sizeQuery.exec())
            emit dbError(sizeQuery.lastError().text());
        qint32 roomSize = 0;
        if(sizeQuery.next()){
            roomSize = sizeQuery.value(0).toInt();
        }

        roomObj[ROOM_SIZE]=roomSize;
        roomsList.append(roomObj);
    }

    QJsonObject response;
    response[REQUEST] = LIST_ROOMS;
    response[ROOMS] = roomsList;
    QJsonDocument doc;
    doc.setObject(response);

    qWarning() << "Emitting listRooms from " << QThread::currentThreadId();
    emit listRoomsResult(doc.toJson(QJsonDocument::Compact), socket);

    emit(t->setThreadStatus(m_id, true)); //this must always be the last
}
