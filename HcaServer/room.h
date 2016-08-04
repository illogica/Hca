#ifndef ROOM_H
#define ROOM_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include "protocol.h"
#include "client.h"
//#include "world.h"

class World;
class Conversation;

class Room : public QObject
{
    Q_OBJECT
    //Q_FORWARD_DECLARE_OBJC_CLASS(Conversation);
public:
    explicit Room(QObject *parent = 0);
    ~Room(){
        while(!m_masters.isEmpty()) delete m_masters.takeFirst();
        while(!m_clients.isEmpty()) delete m_clients.takeFirst();
        //while(!m_conversations.isEmpty()) delete m_conversations.takeFirst();
    }

    QJsonObject toJsonObject();

    bool isEmpty(){return m_clients.isEmpty();}
    int size(){return m_clients.size();}

    void addClient(Client* client);
    bool removeClient(Client* client);

    QString name() const;
    void setName(const QString &name);

    Client *owner() const;
    void setOwner(Client *owner);

    QString description() const;
    void setDescription(const QString &description);

    QString motd() const;
    void setMotd(const QString &motd);

    int id() const;
    void setId(int id);

    World *world() const;
    void setWorld(World *world);

    int count() const;
    void setCount(int count);

    QString avatar() const;
    void setAvatar(const QString &avatar);

signals:

public slots:
    void notifyJoin(const QString &name);
    void notifyLeave(const QString &name);
    void notifyChangeName(const QString &name);


private:
    int m_id;
    int m_count;
    QString m_name;
    QString m_description;
    QString m_motd;
    QString m_avatar;
    World* m_world;
    Client* m_owner;

    QList<Client *> m_masters;  //room operators
    QList<Client *> m_clients;  //room users (and operators, too)
    QList<Conversation *> m_conversations;
};

#endif // ROOM_H
