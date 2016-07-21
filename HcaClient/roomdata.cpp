#include "roomdata.h"

RoomData::RoomData(QObject *parent) : QObject(parent)
{

}

RoomData::RoomData(const QString &name, const QString &description, int size, QObject *parent)
    : QObject(parent), m_name(name), m_description(description), m_size(size)
{
}

void RoomData::setName(const QString &name)
{
    if(m_name!=name){
        m_name = name;
        emit nameChanged();
    }
}

void RoomData::setDescription(const QString &description)
{
    if(m_description != description){
        m_description = description;
        emit descriptionChanged();
    }
}

void RoomData::setSize(int size)
{
    if(m_size != size){
        m_size = size;
        emit sizeChanged();
    }
}

QString RoomData::name()
{
    return m_name;
}

QString RoomData::description()
{
    return m_description;
}

int RoomData::size()
{
    return m_size;
}
