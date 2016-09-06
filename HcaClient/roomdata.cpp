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
        emit nameChanged(m_name);
    }
}

void RoomData::setDescription(const QString &description)
{
    if(m_description != description){
        m_description = description;
        emit descriptionChanged(m_description);
    }
}

void RoomData::setSize(int size)
{
    if(m_size != size){
        m_size = size;
        emit sizeChanged(m_size);
    }
}

int RoomData::id() const
{
    return m_id;
}

void RoomData::setId(int id)
{
    if(m_id != id){
        m_id = id;
        emit idChanged(m_id);
    }
}

QString RoomData::avatar() const
{
    return m_avatar;
}

void RoomData::setAvatar(const QString &avatar)
{
    if(m_avatar != avatar){
        m_avatar = avatar;
        emit avatarChanged(m_avatar);
    }
}

QString RoomData::motd() const
{
    return m_motd;
}

void RoomData::setMotd(const QString &motd)
{
    if(m_motd != motd){
        m_motd = motd;
        emit motdChanged(m_motd);
    }

}

QString RoomData::name() const
{
    return m_name;
}

QString RoomData::description() const
{
    return m_description;
}

int RoomData::size() const
{
    return m_size;
}
