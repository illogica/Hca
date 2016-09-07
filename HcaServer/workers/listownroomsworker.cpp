#include "listownroomsworker.h"

ListOwnRoomsWorker::doWork(HcaThread *t)
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
    response[REQUEST] = LIST_OWN_ROOMS;
    response[ROOMS] = roomsArray;
    QJsonDocument doc;
    doc.setObject(response);

    while(!rooms.isEmpty()) delete rooms.takeFirst();

    emit listOwnRoomsWorkerResult(doc.toJson(QJsonDocument::Compact), socket);
    emit(t->setThreadStatus(m_id, true)); //this must always be the last

    //TODO: CREATE THE SQL QUERY FOR THIS!!!
}
