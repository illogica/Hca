#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtSql>
#include <QThread>
#include "client.h"
#include "world.h"

class DbManager : public QObject
{
    Q_OBJECT
public:
    DbManager();
    ~DbManager(){}
    void init(QSqlDatabase db);
    bool findUserByUuid(const QString& uuid, Client* c);
    bool findUserById(int id, Client* c);
    bool findRoomById(int id, Room* r);
    bool findWorldById(int id, World* w);

    bool isClientInRoom(int clientId, int roomId);
    bool addClientToRoom(int clientId, int roomId);
    void updateClientRoomLastSeen(int clientId, int roomId);
    bool removeClientFromRoom(int clientId, int roomId);

    void createUser(Client* c);
    bool createRoom(QString& name, QString& description, QString& motd, int worldId, int ownerId, QString& avatar, Room* r);
    void updateClientStatus(int id, int status);
    void updateClient(Client* c);

    void listWorlds(QList<World*>& worlds);
    void listRoomsByWorld(int worldId, QList<Room *>& rooms);
    void listRoomsByUser(int userId, QList<Room *>& rooms);
    int countRooms(int worldId);
    int countClients(int roomId);

signals:

public slots:

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
