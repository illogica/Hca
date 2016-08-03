#include "world.h"
#include "protocol.h"


World::World(QObject *parent) : QObject(parent)
{}

QJsonObject World::toJsonObject()
{
    QJsonObject o;
    o[WORLD_ID] = m_id;
    o[WORLD_NAME] = m_name;
    o[DESCRIPTION] = m_description;
    o[WORLD_SIZE] = m_rooms.size();
    o[WORLD_COUNT] = m_count;
    return o;
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

void World::deleteRoom(Room *r)
{
    m_rooms.removeOne(r);
    r->deleteLater();
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

int World::id() const
{
    return m_id;
}

void World::setId(int id)
{
    m_id = id;
}

int World::count() const
{
    return m_count;
}

void World::setCount(int count)
{
    m_count = count;
}
