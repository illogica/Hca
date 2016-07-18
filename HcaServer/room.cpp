#include "room.h"

quint32 Room::idCounter = 0;

Room::Room(QObject *parent) : QObject(parent)
{
    id = idCounter++;
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
