#include "hcaserver.h"
#include <QElapsedTimer>

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
        qWarning() << "Server listening on port " << PORT;
    }
}

void HcaServer::onNewConnection()
{
    QWebSocket *socket = socketServer->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived, this, &HcaServer::onTextMessage);
    connect(socket, &QWebSocket::disconnected, this, &HcaServer::onSocketDisconnected);
    limbo << socket;
    qWarning() << "Added socket to the limbo" << socket->peerName() << ":" << socket->peerPort();
}

void HcaServer::onConnectionClosed()
{
    qWarning() << "Connection closed.";
}

void HcaServer::onTextMessage(QString msg)
{
    QElapsedTimer totalTime;
    totalTime.start();

    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    qWarning() << "received: " << msg << "from " << socket->peerName();

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
    {
        QJsonObject response;
        response["r"] = PONG;
        QJsonDocument doc;
        doc.setObject(response);
        socket->sendTextMessage(doc.toJson());
    }
        break;
    case PONG:
        break;
    default:
        qWarning() << "unrecognized command";
    }

    qWarning() << "Request evaded in " << totalTime.elapsed() << "ms";
    //socket->sendTextMessage(msg);
}

void HcaServer::onSocketDisconnected()
{
    qWarning() << "disconnected.";
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    limbo.removeOne(socket);
}
