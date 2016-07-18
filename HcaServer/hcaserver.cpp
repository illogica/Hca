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

Client *HcaServer::findClient(QUuid uuid)
{
    for(Client *c: clients){
        if(c->uuid() == uuid)
            return c;
    }
    return nullptr; //tbtested
}

Client *HcaServer::findClient(QWebSocket *websocket)
{
    for(Client *c: clients){
        if(c->socket() == websocket)
            return c;
    }
    return nullptr; //tbtested
}

Client *HcaServer::createClient()
{
    Client *c = new Client();
    clients.append(c);
    c->setUuid(QUuid::createUuid());
    return c;
}

Room *HcaServer::findRoom(const QString &name)
{
    for(Room *r : rooms){
        if(r->name() == name)
            return r;
    }
    return nullptr;
}

Room *HcaServer::createRoom()
{
    return new Room();
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
    int r = docObj[REQUEST].toInt();
    switch(r){
    case PING:
    {
        QJsonObject response;
        response[REQUEST] = PONG;
        QJsonDocument doc;
        doc.setObject(response);
        socket->sendTextMessage(doc.toJson());
    }
        break;

    case LOGIN:
    {
        //Extract data from the json request
        QUuid uuid(docObj[UUID].toString());

        Client *c = findClient(uuid);
        if(!c){
            c = createClient();
        }
        c->setSocket(socket);
        if(!onlineClients.contains(c)){
            onlineClients.append(c);
        }
        limbo.removeOne(socket);

        QJsonObject response;
        response[REQUEST] = LOGIN;
        response[NAME] = c->name();
        response[UUID] = c->uuid().toString();
        QJsonDocument doc;
        doc.setObject(response);
        emit c->queueTextMessage(doc.toJson());

        qWarning() << "Online clients: " << onlineClients.size();
    }
        break;

    case JOIN_ROOM:
    {
        Client *c = findClient(socket);
        if(!c){
            socket->sendTextMessage(makeErrorMessage("Client not found"));
            return;
        }

        QString roomName = docObj[ROOM_NAME].toString();
        Room *r = findRoom(roomName);
        if(!r){
            r = createRoom();
            r->setName(roomName);
            r->setOwner(c);
        }

        /*FINIRE QUI CAZZO*/
        QJsonObject response;
        response[REQUEST] = LOGIN;
        response[NAME] = c->name();
        response[UUID] = c->uuid().toString();
        QJsonDocument doc;
        doc.setObject(response);
        emit c->queueTextMessage(doc.toJson());

        qWarning() << "Online clients: " << onlineClients.size();
    }
        break;

    case PONG:
        break;
    default:
        qWarning() << "unrecognized command";
    }

    qWarning() << "Request evaded in " << totalTime.elapsed() << "ms";
}

void HcaServer::onSocketDisconnected()
{
    qWarning() << "disconnected socket";
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    limbo.removeOne(socket); //even if it's not there
    onlineClients.removeOne(findClient(socket));
    qWarning() << "Online clients: " << onlineClients.size();
}

QJsonDocument HcaClient::makeError(const QString &error)
{
    QJsonObject response;
    response[REQUEST] = ERROR;
    response[ERROR_BODY] = error;
    QJsonDocument doc;
    doc.setObject(response);
    return doc;
}
