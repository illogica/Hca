#include "roomdata.h"

RoomData::RoomData(QObject *parent) : QObject(parent)
{

}

void RoomData::setName(const QString &name)
{
    m_name = name;
    emit nameChanged(name);
}

void RoomData::setDescription(const QString &description)
{
    m_description = description;
    emit descriptionChanged(m_description);
}

void RoomData::setSize(int size)
{
    m_size = size;
    emit sizeChanged(m_size);
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
