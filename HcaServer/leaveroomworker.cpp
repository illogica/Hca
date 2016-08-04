#include "leaveroomworker.h"

void LeaveRoomWorker::doWork(HcaThread *t)
{
    m_id = t->id();
    DbManager* dbm = t->dbManager();
    Q_UNUSED(dbm)
    emit(t->setThreadStatus(m_id, false)); //this must always be the first

    bool ok = dbm->removeClientFromRoom(clientId, roomdId);

    QJsonObject response;
    response[REQUEST] = LEAVE_ROOM;
    response[ROOM_ID] = roomdId;
    response[RESULT] = ok ? DONE : DENIED;

    QJsonDocument doc;
    doc.setObject(response);

    emit leaveRoomResult(doc.toJson(QJsonDocument::Compact), socket);
    emit(t->setThreadStatus(m_id, true)); //this must always be the last
}
