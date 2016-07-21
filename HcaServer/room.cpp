#include "room.h"

quint32 Room::idCounter = 0;

Room::Room(QObject *parent) : QObject(parent)
{
    id = idCounter++;
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

QString Room::description() const
{
    return m_description;
}

void Room::setDescription(const QString &description)
{
    m_description = description;
}
