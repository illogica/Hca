#include "createroomworker.h"
#include "world.h"
//#include "room.h"

void CreateRoomWorker::doWork(HcaThread* t)
{
    m_id = t->id();
    DbManager* dbm = t->dbManager();
    emit(t->setThreadStatus(m_id, false)); //this must always be the first

    QScopedPointer<World> w(new World());
    QScopedPointer<Room> r(new Room());
    QJsonObject response;
    response[REQUEST] = CREATE_ROOM;

    bool ok = false;

    if(dbm->findWorldById(worldId, w.data())){
        if(dbm->createRoom(name, description, motd, worldId, ownerId, avatar, r.data())){
            if(dbm->addClientToRoom(ownerId, r->id())){
                ok = true;
            }
        }
    }
    response[RESULT] = ok ? DONE : DENIED;
    QJsonDocument doc;
    doc.setObject(response);
    emit createRoomResult(doc.toJson(QJsonDocument::Compact), socket);
    emit(t->setThreadStatus(m_id, true)); //this must always be the last
}
