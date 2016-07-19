#include "world.h"

quint32 World::idCounter = 0;

World::World(QObject *parent) : QObject(parent)
{

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
