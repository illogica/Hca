#include "client.h"
#include "protocol.h"

Client::Client(QObject *parent) : QObject(parent)
{
    connect(this, &Client::queueTextMessage, this, &Client::sendTextMessage);
    setName(QStringLiteral("unnamed"));
}

QJsonObject Client::toFullJsonObject()
{
    QJsonObject o;
    o[ID] = m_id;
    o[NAME] = m_name;
    o[UUID] = m_uuid;
    o[DESCRIPTION] = m_description;
    o[STATUS] = m_status;
    o[PRIVILEGE] = m_privilege;
    o[AVATAR] = m_avatar;
    return o;
}

QJsonObject Client::toLightJsonObject()
{
    QJsonObject o;
    o[ID] = m_id;
    o[NAME] = m_name;
    o[DESCRIPTION] = m_description;
    o[STATUS] = m_status;
    o[PRIVILEGE] = m_privilege;
    o[AVATAR] = m_avatar;
    return o;
}

QString Client::uuid() const
{
    return m_uuid;
}

void Client::setUuid(const QString &uuid)
{
    m_uuid = uuid;
}

QString Client::name() const
{
    return m_name;
}

void Client::setName(const QString &name)
{
    m_name = name;
}

QWebSocket *Client::socket() const
{
    return m_socket;
}

void Client::setSocket(QWebSocket *socket)
{
    m_socket = socket;
}

void Client::sendTextMessage(const QString &message)
{
    m_socket->sendTextMessage(message);
}

QString Client::description() const
{
    return m_description;
}

void Client::setDescription(const QString &description)
{
    m_description = description;
}

int Client::privilege() const
{
    return m_privilege;
}

void Client::setPrivilege(int privilege)
{
    m_privilege = privilege;
}

int Client::status() const
{
    return m_status;
}

void Client::setStatus(int status)
{
    m_status = status;
}

int Client::id() const
{
    return m_id;
}

void Client::setId(int id)
{
    m_id = id;
}

QString Client::avatar() const
{
    return m_avatar;
}

void Client::setAvatar(const QString &avatar)
{
    m_avatar = avatar;
}

