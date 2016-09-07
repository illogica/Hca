#include "dbmanager.h"
#include "protocol.h"
#include <QUuid>

DbManager::DbManager()
{}

void DbManager::init(QSqlDatabase db)
{
    m_db = db;
}

bool DbManager::findUserByUuid(const QString &uuid, Client* c)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM clients WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);
    if(!query.exec()){
        qWarning() << "Error in " << query.lastQuery();
        qWarning() << "ERROR: " << query.lastError().text();
        return false;
    }
    if(query.next()){
        c->setId(query.value(0).toInt());
        c->setUuid(query.value(1).toString());
        c->setName(query.value(2).toString());
        c->setDescription(query.value(3).toString());
        c->setPrivilege(query.value(4).toInt());
        c->setStatus(query.value(5).toInt());
        c->setAvatar(query.value(6).toString());
        qWarning() << "User found " << c->id();
        return true;
    } return false;
}

bool DbManager::findUserById(int id, Client* c)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM clients WHERE id = :id");
    query.bindValue(":id", id);
    if(!query.exec()){
        qWarning() << "Error in " << query.lastQuery();
        qWarning() << "ERROR: " << query.lastError().text();
        return false;
    }
    if(query.next()){
        c->setId(query.value(0).toInt());
        c->setUuid(query.value(1).toString());
        c->setName(query.value(2).toString());
        c->setDescription(query.value(3).toString());
        c->setPrivilege(query.value(4).toInt());
        c->setStatus(query.value(5).toInt());
        c->setAvatar(query.value(6).toString());
        qWarning() << "User found " << c->id();
        return true;
    } return false;
}

bool DbManager::findRoomById(int id, Room *r)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM rooms WHERE id = :id");
    query.bindValue(":id", id);
    if(!query.exec()){
        qWarning() << "Error in " << query.lastQuery();
        qWarning() << "ERROR: " << query.lastError().text();
        return false;
    }
    if(query.next()){
        r->setId(query.value(0).toInt());
        r->setName(query.value(1).toString());
        r->setDescription(query.value(2).toString());
        r->setMotd(query.value(3).toString());
        r->setAvatar(query.value(6).toString());
        r->setCount(countClients(r->id()));

        int worldId = query.value(4).toInt();
        int ownerId = query.value(5).toInt();

        QPointer<World> w(new World(r));
        findWorldById(worldId, w.data());
        r->setWorld(w.data());

        QPointer<Client> c(new Client(r));
        findUserById(ownerId, c.data());
        r->setOwner(c.data());

        qWarning() << "Room found " << r->id();
        return true;
    } return false;
}

bool DbManager::findWorldById(int id, World *w)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM worlds WHERE id = :id");
    query.bindValue(":id", id);
    if(!query.exec()){
        qWarning() << "Error in " << query.lastQuery();
        qWarning() << "ERROR: " << query.lastError().text();
        return false;
    }
    if(query.next()){
        w->setId(query.value(0).toInt());
        w->setName(query.value(1).toString());
        w->setDescription(query.value(2).toString());
        qWarning() << "World found " << w->id();
        return true;
    } return false;
}

bool DbManager::isClientInRoom(int clientId, int roomId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM roomclients WHERE roomid = :roomId AND clientid = :clientId");
    query.bindValue(":roomId", roomId);
    query.bindValue(":clientId", clientId);
    if(!query.exec()){
        qWarning() << "Error in " << query.lastQuery();
        qWarning() << "ERROR: " << query.lastError().text();
        return false;
    }
    if(query.next()){
        qWarning() << "User already in room.";
        return true;
    } return false;
}

bool DbManager::addClientToRoom(int clientId, int roomId)
{
    QSqlQuery insQuery(m_db);
    insQuery.prepare("INSERT INTO roomclients(roomid, clientid, lastseen) VALUES (:roomid, :clientid, now())");
    insQuery.bindValue(":roomid", roomId);
    insQuery.bindValue(":clientid", clientId);
    if(!insQuery.exec()){
        qWarning() << "ERROR: " << insQuery.lastError().text();
        return false;
    } else {
        qWarning() << "User created";
        return true;
    }
}

void DbManager::updateClientRoomLastSeen(int clientId, int roomId)
{
    QSqlQuery updQuery(m_db);
    updQuery.prepare("UPDATE roomclients SET lastseen = now() WHERE roomid = :roomId AND clientid = :clientId");
    updQuery.bindValue(":roomId", roomId);
    updQuery.bindValue(":clientId", clientId);
    if(!updQuery.exec()){
        qWarning() << "ERROR: " << updQuery.lastError().text();
    } //user found and updated
}

bool DbManager::removeClientFromRoom(int clientId, int roomId)
{
    QSqlQuery delQuery(m_db);
    delQuery.prepare("DELETE FROM roomclients WHERE roomid = :roomid AND clientid = :clientid)");
    delQuery.bindValue(":roomid", roomId);
    delQuery.bindValue(":clientid", clientId);
    if(!delQuery.exec()){
        qWarning() << "ERROR: " << delQuery.lastError().text();
        return false;
    } else {
        qWarning() << "User left room";
        return true;
    }
}

void DbManager::createUser(Client *c)
{
    c->setName(QStringLiteral("unnamed"));
    c->setDescription(QStringLiteral(""));
    c->setPrivilege(USER);
    c->setStatus(ONLINE);
    c->setUuid(QUuid::createUuid().toString());
    c->setAvatar(QStringLiteral("1f42a"));

    QSqlQuery insQuery(m_db);
    insQuery.prepare("INSERT INTO clients(uuid, name, description, privilege, status, avatar) VALUES (:uuid, :name, :description, :privilege, :status, :avatar) RETURNING id");
    insQuery.bindValue(":uuid", c->uuid());
    insQuery.bindValue(":name", c->name());
    insQuery.bindValue(":description", c->description());
    insQuery.bindValue(":privilege", c->privilege());
    insQuery.bindValue(":status", c->status());
    insQuery.bindValue(":avatar", c->avatar());
    if(!insQuery.exec()){
        qWarning() << "Error in " << insQuery.lastQuery();
        qWarning() << "ERROR: " << insQuery.lastError().text();
    } else {
        insQuery.next();
        c->setId(insQuery.value(0).toInt());
        qWarning() << "User created";
    }
}

bool DbManager::createRoom(QString &name, QString &description, QString &motd, int worldId, int ownerId, QString &avatar, Room *r)
{
    r->setName(name);
    r->setDescription(description);
    r->setMotd(motd);
    QSqlQuery insQuery(m_db);
    insQuery.prepare("INSERT INTO rooms(name, description, motd, worldid, ownerid, avatar) VALUES (:name, :description, :motd, :worldid, :ownerid, :avatar) RETURNING id");
    insQuery.bindValue(":name", name);
    insQuery.bindValue(":description", description);
    insQuery.bindValue(":motd", motd);
    insQuery.bindValue(":worldid", worldId);
    insQuery.bindValue(":ownerid", ownerId);
    insQuery.bindValue(":avatar", avatar);
    if(!insQuery.exec()){
        qWarning() << "Error in " << insQuery.lastQuery();
        qWarning() << "ERROR: " << insQuery.lastError().text();
        return false;
    } else {
        if(insQuery.next()){
            r->setId(insQuery.value(0).toInt());
            qWarning() << "Room created";
            return true;
        }
    } return false;
}

void DbManager::updateClientStatus(int id, int status)
{
    QSqlQuery updQuery(m_db);
    updQuery.prepare("UPDATE clients SET status = :st WHERE id = :id");
    updQuery.bindValue(":st", status);
    updQuery.bindValue(":id", id);
    if(!updQuery.exec()){
        qWarning() << "ERROR: " << updQuery.lastError().text();
    } //user found and updated
}

void DbManager::updateClient(Client *c)
{
    QSqlQuery updQuery(m_db);
    updQuery.prepare("UPDATE clients SET uuid = :uuid, name = :name, description = :description, privilege = :privilege, status = :status, avatar = :avatar WHERE id = :id");
    updQuery.bindValue(":uuid", c->uuid());
    updQuery.bindValue(":name", c->name());
    updQuery.bindValue(":description", c->description());
    updQuery.bindValue(":privilege", c->privilege());
    updQuery.bindValue(":status", c->status());
    updQuery.bindValue(":avatar", c->avatar());

    updQuery.bindValue(":id", c->id());
    if(!updQuery.exec()){
        qWarning() << "ERROR: " << updQuery.lastError().text();
    } //user found and updated
}

void DbManager::listWorlds(QList<World *>& worlds)
{

    QSqlQuery query("SELECT * FROM worlds", m_db);
    while(query.next()){
        QPointer<World> w(new World());
        w->setId(query.value(0).toInt());
        w->setName(query.value(1).toString());
        w->setDescription(query.value(2).toString());
        w->setCount(countRooms(w->id()));
        worlds.append(w.data());
    }
}

void DbManager::listRoomsByWorld(int worldId, QList<Room *>& rooms)
{

    QPointer<World> w(new World());
    findWorldById(worldId, w.data());

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM rooms WHERE worldid = :worldId");
    query.bindValue(":worldId", worldId);
    if(!query.exec()){
        qWarning() << "Error in " << query.lastQuery();
        qWarning() << "ERROR: " << query.lastError().text();
    }
    while(query.next()){
        QPointer<Room> r(new Room(w));
        r->setId(query.value(0).toInt());
        r->setName(query.value(1).toString());
        r->setDescription(query.value(2).toString());
        r->setMotd(query.value(3).toString());
        r->setAvatar(query.value(6).toString());
        r->setCount(countClients(r->id()));

        QPointer<Client> c(new Client(r));
        findUserById(query.value(5).toInt(), c.data());
        r->setOwner(c.data());

        r->setWorld(w);

        rooms.append(r.data());
    }
}

void DbManager::listRoomsByUser(int userId, QList<Room *> &rooms)
{

}

int DbManager::countRooms(int worldId)
{
    QSqlQuery sizeQuery(m_db);
    sizeQuery.prepare("SELECT COUNT(*) FROM rooms WHERE rooms.worldid = :id");
    sizeQuery.bindValue(":id", worldId);
    if(!sizeQuery.exec()){
        qWarning() << "ERROR: " << sizeQuery.lastError().text();
        return 0;
    }
    if(sizeQuery.next()){
        return sizeQuery.value(0).toInt();
    } else {
        qWarning() << "ERROR: world " << worldId << " not found "<< __FILE__ << __LINE__;
        return 0;
    }
}

int DbManager::countClients(int roomId)
{
    QSqlQuery sizeQuery(m_db);
    sizeQuery.prepare("SELECT COUNT(*) FROM roomclients WHERE roomid = :id");
    sizeQuery.bindValue(":id", roomId);
    if(!sizeQuery.exec()){
        qWarning() << "ERROR: " << sizeQuery.lastError().text();
        return 0;
    }
    if(sizeQuery.next()){
        return sizeQuery.value(0).toInt();
    } else {
        qWarning() << "ERROR: room " << roomId << " not found "<< __FILE__ << __LINE__;
        return 0;
    }
}
