#include "joinroomworker.h"

void JoinRoomWorker::doWork(HcaThread *t)
{
    m_id = t->id();
    DbManager* dbm = t->dbManager();
    emit(t->setThreadStatus(m_id, false)); //this must always be the first

    bool ok = dbm->addClientToRoom(clientId, roomId);

    QJsonObject response;
    response[REQUEST] = JOIN_ROOM;
    response[ROOM_ID] = roomId;
    response[RESULT] = ok ? DONE : DENIED;
    QJsonDocument doc;
    doc.setObject(response);

    emit joinRoomResult(doc.toJson(QJsonDocument::Compact), socket);
    emit(t->setThreadStatus(m_id, true)); //this must always be the last
}
