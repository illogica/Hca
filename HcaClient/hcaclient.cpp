#include "hcaclient.h"
#include "QUuid"
#include "../HcaServer/protocol.h"

HcaClient::HcaClient(QObject *parent) : QObject(parent)
{
    connect(&socket, &QWebSocket::connected, this, &HcaClient::onConnected);
    connect(&socket, &QWebSocket::disconnected, this, &HcaClient::onDisconnected);
    connect(&socket, &QWebSocket::textMessageReceived, this, &HcaClient::parseServerMessage);
    //connect();
}

void HcaClient::establish(){
    socket.open(QUrl(QStringLiteral("ws://localhost:8081")));
}

void HcaClient::onConnected()
{
    qWarning() << "onConnected()";
    m_connected = true;
    emit connectedChanged(true);
    socket.sendTextMessage(makePing().toJson(QJsonDocument::Compact));
}

void HcaClient::onDisconnected()
{
    qWarning() << "onDisconnected()";
    m_connected = false;
    emit connectedChanged(false);
}

void HcaClient::parseServerMessage(const QString &message)
{
    qWarning() << "Received: " << message;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if(!doc.isObject()){
        qWarning() << "Invalid json: " << message;
        return;
    }

    QJsonObject docObj = doc.object();
    int r = docObj[REQUEST].toInt();
    switch(r){
    case PING:
    {
        QJsonObject response;
        response[REQUEST] = PONG;
        QJsonDocument doc;
        doc.setObject(response);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
        break;
    case LOGIN:
    {
        QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
        QUuid uuid(docObj[UUID].toString());
        settings.setValue(UUID, uuid);
        settings.setValue(NAME, docObj[NAME].toString());
        qWarning() << "new Uuid: " << uuid.toString();
    }
        break;
    default:
        qWarning() << "unrecognized command";
    }
}

void HcaClient::sendLogin()
{
    if(m_connected){
        QJsonObject response;
        response[REQUEST] = LOGIN;
        response[UUID] = settings.value(UUID, "").toUuid().toString();
        QJsonDocument doc;
        doc.setObject(response);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

bool HcaClient::connected(){ return m_connected;}

void HcaClient::setConnected(bool cntd)
{
    if(cntd && !m_connected){
        establish();
        return;
    }
    if(!cntd && m_connected){
        socket.close();
    }
}

QJsonDocument HcaClient::makePing()
{
    QJsonObject response;
    response[REQUEST] = PONG;
    QJsonDocument doc;
    doc.setObject(response);
    return doc;
}
