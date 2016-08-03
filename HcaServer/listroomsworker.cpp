#include "listroomsworker.h"
#include "protocol.h"

ListRoomsWorker::ListRoomsWorker(){}

void ListRoomsWorker::doWork(HcaThread *t)
{
    m_id = t->id();
    DbManager* dbm = t->dbManager();
    emit(t->setThreadStatus(m_id, false)); //this must always be the first

    QJsonArray roomsArray;
    QList<Room*> rooms;
    dbm->listRoomsByWorld(worldId, rooms);
    for(Room* r: rooms){
        roomsArray.append(r->toJsonObject());
    }

    QJsonObject response;
    response[REQUEST] = LIST_ROOMS;
    response[ROOMS] = roomsArray;
    QJsonDocument doc;
    doc.setObject(response);

    while(!rooms.isEmpty()) delete rooms.takeFirst();

    qWarning() << "Emitting listRooms from " << QThread::currentThreadId();
    emit listRoomsResult(doc.toJson(QJsonDocument::Compact), socket);

    emit(t->setThreadStatus(m_id, true)); //this must always be the last
}
