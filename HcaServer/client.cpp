#include "client.h"

quint32 Client::idCounter = 0;

Client::Client(QObject *parent) : QObject(parent)
{
    id = idCounter++;
    connect(this, &Client::queueTextMessage, this, &Client::sendTextMessage);
    setName(QStringLiteral("unnamed"));
}

QUuid Client::uuid() const
{
    return m_uuid;
}

void Client::setUuid(const QUuid &uuid)
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

