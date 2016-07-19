#include "roomdata.h"

RoomData::RoomData(QObject *parent) : QObject(parent)
{

}

void RoomData::setRoomName(const QString &name)
{
    m_name = name;
    emit roomNameChanged(name);
}

void RoomData::setRoomClients(int clients)
{
    m_clients = clients;
    emit roomClientsChanged(clients);
}

QString RoomData::roomName()
{
    return m_name;
}

int RoomData::roomClients()
{
    return m_clients;
}
