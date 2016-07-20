#include "worlddata.h"

WorldData::WorldData(QObject *parent) : QObject(parent)
{

}

void WorldData::setName(const QString &name)
{
    m_name = name;
    emit nameChanged(name);
}

void WorldData::setDescription(const QString &description)
{
    m_description = description;
    emit descriptionChanged(m_description);
}

void WorldData::setSize(int size)
{
    m_size = size;
    emit sizeChanged(m_size);
}

QString WorldData::name()
{
    return m_name;
}

QString WorldData::description()
{
    return m_description;
}

int WorldData::size()
{
    return m_size;
}
