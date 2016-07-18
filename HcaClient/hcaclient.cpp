#include "hcaclient.h"
#include "../HcaServer/protocol.h"

HcaClient::HcaClient(QObject *parent) : QObject(parent)
{
    connect(&socket, &QWebSocket::connected, this, &HcaClient::onConnected);
    connect(&socket, &QWebSocket::disconnected, this, &HcaClient::onDisconnected);
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
    socket.sendTextMessage(makePing().toJson());
}

void HcaClient::onDisconnected()
{
    qWarning() << "onDisconnected()";
    m_connected = false;
    emit connectedChanged(false);
}

void HcaClient::parseServerMessage(QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if(!doc.isObject()){
        qWarning() << "Invalid json: " << message;
        return;
    }

    QJsonObject docObj = doc.object();
    int r = docObj["r"].toInt();
    switch(r){
    case PING:
    {
        QJsonObject response;
        response["r"] = PONG;
        QJsonDocument doc;
        doc.setObject(response);
        socket.sendTextMessage(doc.toJson());
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
        response["r"] = LOGIN;
        response["e"] = "gino.pilotino@gmail.com";
        QJsonDocument doc;
        doc.setObject(response);
        socket.sendTextMessage(doc.toJson());
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
    response["r"] = PONG;
    QJsonDocument doc;
    doc.setObject(response);
    return doc;
}
