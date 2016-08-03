#include "room.h"

Room::Room(QObject *parent) : QObject(parent)
{}

QJsonObject Room::toJsonObject()
{
    QJsonObject o;
    o[ROOM_ID] = m_id;
    o[ROOM_NAME] = m_name;
    o[DESCRIPTION] = m_description;
    o[ROOM_MOTD] = m_motd;
    o[ROOM_SIZE] = m_clients.size();
    o[ROOM_COUNT] = m_count;
    return o;
}

void Room::addClient(Client *client)
{
    if(!m_clients.contains(client)){
        m_clients.append(client);
        notifyJoin(client->name());
    }
}

bool Room::removeClient(Client *client)
{
    if(m_clients.removeOne(client)){
        notifyLeave(client->name());
        return true;
    } else {
        return false;
    }
}

QString Room::name() const
{
    return m_name;
}

void Room::setName(const QString &name)
{
    m_name = name;
}

Client *Room::owner() const
{
    return m_owner;
}

void Room::setOwner(Client *owner)
{
    m_owner = owner;
}

void Room::notifyJoin(const QString &name)
{
    QJsonObject response;
    response[REQUEST] = NOTIFY_JOIN_ROOM;
    response[NAME] = name;
    QJsonDocument doc;
    doc.setObject(response);
    for(Client *c : m_clients){
        if(c->name() != name)
            emit c->queueTextMessage(doc.toJson());
    }
}

void Room::notifyLeave(const QString &name)
{
    QJsonObject response;
    response[REQUEST] = NOTIFY_LEAVE_ROOM;
    response[NAME] = name;
    QJsonDocument doc;
    doc.setObject(response);
    for(Client *c : m_clients){
        if(c->name() != name)
            emit c->queueTextMessage(doc.toJson());
    }
}

void Room::notifyChangeName(const QString &name)
{
    QJsonObject response;
    response[REQUEST] = NOTIFY_CHANGE_ROOM_NAME;
    response[NAME] = name;
    QJsonDocument doc;
    doc.setObject(response);
    for(Client *c : m_clients){
        emit c->queueTextMessage(doc.toJson());
    }
}

int Room::count() const
{
    return m_count;
}

void Room::setCount(int count)
{
    m_count = count;
}

World *Room::world() const
{
    return m_world;
}

void Room::setWorld(World *world)
{
    m_world = world;
}

int Room::id() const
{
    return m_id;
}

void Room::setId(int id)
{
    m_id = id;
}

QString Room::motd() const
{
    return m_motd;
}

void Room::setMotd(const QString &motd)
{
    m_motd = motd;
}

QString Room::description() const
{
    return m_description;
}

void Room::setDescription(const QString &description)
{
    m_description = description;
}
