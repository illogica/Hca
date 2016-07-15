#include "hcaserver.h"

HcaServer::HcaServer(QObject *parent) : QObject(parent)
{
    init();
}

void HcaServer::init()
{
    socketServer = new QWebSocketServer(QStringLiteral("Hca Server"), QWebSocketServer::NonSecureMode, this);
    if(socketServer->listen(QHostAddress::Any, PORT)){
        connect(socketServer, &QWebSocketServer::newConnection, this, &HcaServer::onNewConnection);
        connect(socketServer, &QWebSocketServer::closed, this, &HcaServer::onConnectionClosed);
        qDebug() << "Server listening on port " << PORT;
    }
}

void HcaServer::onNewConnection()
{
    QWebSocket *socket = socketServer->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived, this, &HcaServer::onTextMessage);
    connect(socket, &QWebSocket::disconnected, this, &HcaServer::onSocketDisconnected);
    limbo << socket;
    qDebug() << "Added client" << socket->peerName() << ":" << socket->peerPort();
}

void HcaServer::onConnectionClosed()
{
    qDebug() << "Connection closed.";
}

void HcaServer::onTextMessage(QString msg)
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    qDebug() << "received: " << msg << "from " << socket->peerName();

    //Json
    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    if(!doc.isObject()){
        socket->sendTextMessage("json or gtfo");
        return;
    }

    QJsonObject docObj = doc.object();
    int r = docObj["r"].toInt();
    switch(r){
    case PING:
        qDebug() << "sent ping";
        break;
    default:
        qDebug() << "unrecognized command";
    }

    socket->sendTextMessage(msg);
}

void HcaServer::onSocketDisconnected()
{
    qDebug() << "disconnected.";
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    limbo.removeOne(socket);
}
