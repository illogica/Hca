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
    sockets << socket;
    qDebug() << "Added client" << socket->peerName() << ":" << socket->peerPort();
}

void HcaServer::onConnectionClosed()
{

}

void HcaServer::onTextMessage(QString msg)
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    qDebug() << "received: " << msg << "from " << socket->peerName();
    socket->sendTextMessage(msg);
}

void HcaServer::onSocketDisconnected()
{
    qDebug() << "disconnected.";
}
