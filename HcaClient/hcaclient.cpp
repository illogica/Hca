#include "hcaclient.h"
#include "../HcaServer/protocol.h"

HcaClient::HcaClient(QObject *parent) : QObject(parent)
{
    connect(&socket, &QWebSocket::connected, this, &HcaClient::onConnected);
    connect(&socket, &QWebSocket::disconnected, this, &HcaClient::onDisconnected);
    socket.open(QUrl(QStringLiteral("ws://localhost:8081")));
}

void HcaClient::onConnected()
{
    qWarning() << "onConnected()";
    socket.sendTextMessage(makePing().toJson());
}

void HcaClient::onDisconnected()
{
    qWarning() << "onDisconnected()";
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

QJsonDocument HcaClient::makePing()
{
    QJsonObject response;
    response["r"] = PONG;
    QJsonDocument doc;
    doc.setObject(response);
    return doc;
}
