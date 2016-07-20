#include "world.h"

quint32 World::idCounter = 0;

World::World(QObject *parent) : QObject(parent)
{
    id = idCounter++;
}

Room *World::findRoom(const QString &name)
{
    for(Room *r : m_rooms){
        if(r->name() == name)
            return r;
    }
    return nullptr;
}

Room *World::createRoom()
{
    Room *r = new Room(this);
    m_rooms << r;
    return r;
}

QString World::name() const
{
    return m_name;
}

void World::setName(const QString &name)
{
    m_name = name;
}

QList<Room *> World::rooms() const
{
    return m_rooms;
}

void World::setRooms(const QList<Room *> &rooms)
{
    m_rooms = rooms;
}

QString World::description() const
{
    return m_description;
}

void World::setDescription(const QString &description)
{
    m_description = description;
}
