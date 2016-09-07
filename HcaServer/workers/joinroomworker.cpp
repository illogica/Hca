#include "joinroomworker.h"

void JoinRoomWorker::doWork(HcaThread *t)
{
    m_id = t->id();
    DbManager* dbm = t->dbManager();
    emit(t->setThreadStatus(m_id, false)); //this must always be the first

    bool ok=false;

    if(dbm->isClientInRoom(clientId, roomId)){
            dbm->updateClientRoomLastSeen(clientId, roomId);
            ok = true;
    } else {
        ok = dbm->addClientToRoom(clientId, roomId);
    }

    QScopedPointer<Room> r(new Room(this));
    dbm->findRoomById(roomId, r.data());

    QJsonObject response;
    response[REQUEST] = JOIN_ROOM;
    response[ROOM_ID] = roomId;
    response[RESULT] = ok ? DONE : DENIED;
    response[ROOM_NAME] = r->name();
    response[DESCRIPTION] = r->description();
    response[ROOM_MOTD] = r->motd();
    response[ROOM_SIZE] = r->size();
    response[ROOM_COUNT] = r->count();
    response[ROOM_AVATAR] = r->avatar();
    QJsonDocument doc;
    doc.setObject(response);

    emit joinRoomResult(doc.toJson(QJsonDocument::Compact), socket);
    emit(t->setThreadStatus(m_id, true)); //this must always be the last
}
