#include "worlddata.h"

WorldData::WorldData(QObject *parent) : QObject(parent)
{
}

WorldData::WorldData(const QString &name, const QString &description, int size, QObject *parent)
    : QObject(parent), m_name(name), m_description(description), m_size(size)
{
}

void WorldData::resetRooms()
{
    qDeleteAll(m_rooms);
    qDeleteAll(m_roomsModel);
}

void WorldData::addRoom(RoomData *r)
{
    m_rooms.append(r);
    m_roomsModel.append(new RoomData(r->name(), r->description(), r->size()));
}

void WorldData::setName(const QString &name)
{
    if(m_name != name){
        m_name = name;
        emit nameChanged();
    }
}

void WorldData::setDescription(const QString &description)
{
    if(m_description != description){
        m_description = description;
        emit descriptionChanged();
    }
}

void WorldData::setSize(int size)
{
    if(m_size != size){
        m_size = size;
        emit sizeChanged();
    }
}

QList<QObject *> WorldData::roomsModel() const
{
    return m_roomsModel;
}

QString WorldData::name() const
{
    return m_name;
}

QString WorldData::description() const
{
    return m_description;
}

int WorldData::size() const
{
    return m_size;
}
