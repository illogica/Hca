#include "hcaserver.h"
#include <QElapsedTimer>
#include "pingrunnable.h"
#include "loginworker.h"
#include "disconnectworker.h"
#include "listworldsworker.h"
#include "listroomsworker.h"

HcaServer::HcaServer(QObject *parent) : QObject(parent)
{
    init();
}

void HcaServer::init()
{
    m_tp = new HcaThreadPool(this);

    if (!QSqlDatabase::drivers().contains("QPSQL")){
        qWarning() << "Unable to load database, PSQL driver missing";
        return;
    }

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
    onlineSockets << socket;
    qWarning() << "Added socket " << socket->peerName() << ":" << socket->peerPort();
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

        PingRunnable *pr = new PingRunnable();
        pr->socket = socket;
        connect(pr, &PingRunnable::pingResult, this, &HcaServer::onPingResult);
        QThreadPool::globalInstance()->start(pr);
    } break;

    case LOGIN:
    {
        QPointer<LoginWorker> w = new LoginWorker();
        w->uuid = docObj[UUID].toString();
        w->socket = socket;
        connect(w, &LoginWorker::loginResult, this, &HcaServer::onLoginResult);
        connect(w, &LoginWorker::loginResult, w, &LoginWorker::deleteLater);
        m_tp->push(w);
    } break;

    case LIST_WORLDS:
    {
        QPointer<ListWorldsWorker> w = new ListWorldsWorker();
        w->socket = socket;
        connect(w, &ListWorldsWorker::listWorldsResult, this, &HcaServer::onListWorldsResult);
        connect(w, &ListWorldsWorker::listWorldsResult, w, &ListWorldsWorker::deleteLater);
        m_tp->push(w);
    } break;

    case LIST_ROOMS:
    {
        qint32 worldId = docObj[WORLD_ID].toInt();
        QPointer<ListRoomsWorker> w = new ListRoomsWorker();
        w->socket = socket;
        w->worldId = worldId;
        connect(w, &ListRoomsWorker::listRoomsResult, this, &HcaServer::onListRoomsResult);
        connect(w, &ListRoomsWorker::listRoomsResult, w, &ListRoomsWorker::deleteLater);
        m_tp->push(w);
    } break;
/*
    case JOIN_ROOM:
    {
        Client *c = findClient(socket);
        if(!c){
            socket->sendTextMessage(makeErrorMessage("Client not found").toJson(QJsonDocument::Compact));
            return;
        }

        QString world = docObj[WORLD_NAME].toString();
        World *w = findWorld(world);
        if(!w){
            socket->sendTextMessage(makeErrorMessage("World not found").toJson(QJsonDocument::Compact));
            return;
        }

        //If room doesn't exist, create it
        QString roomName = docObj[ROOM_NAME].toString();
        Room *r = w->findRoom(roomName);
        if(!r){
            r = w->createRoom();
            r->setName(roomName);
            r->setOwner(c);
        }

        //add user to the room
        r->addClient(c);

        QJsonObject response;
        response[REQUEST] = JOIN_ROOM;
        response[NAME] = r->name();
        QJsonDocument doc;
        doc.setObject(response);
        emit c->queueTextMessage(doc.toJson(QJsonDocument::Compact));

        qWarning() << "Client " << c->name() << " joined room " << r->name();
    } break;

    case LEAVE_ROOM:
    {
        Client *c = findClient(socket);
        if(!c){
            socket->sendTextMessage(makeErrorMessage("Client not found").toJson(QJsonDocument::Compact));
            return;
        }

        QString world = docObj[WORLD_NAME].toString();
        World *w = findWorld(world);
        if(!w){
            socket->sendTextMessage(makeErrorMessage("World not found").toJson(QJsonDocument::Compact));
            return;
        }

        QString roomName = docObj[ROOM_NAME].toString();
        Room *r = w->findRoom(roomName);
        if(r->removeClient(c)){
            QJsonObject response;
            response[REQUEST] = LEAVE_ROOM;
            response[NAME] = r->name();
            QJsonDocument doc;
            doc.setObject(response);
            emit c->queueTextMessage(doc.toJson(QJsonDocument::Compact));

            if(r->isEmpty()){
                w->deleteRoom(r);
            }
            qWarning() << "Client " << c->name() << " left room " << r->name();
        }
    } break;*/

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
    onlineSockets.removeOne(socket); //even if it's not there

    //if there's a client with that socket, set it as "offline"
    if(clientsBySocket.contains(socket)){
        QPointer<DisconnectWorker> w = new DisconnectWorker();
        w->uuid = clientsBySocket.value(socket);
        connect(w, &DisconnectWorker::disconnectResult, this, &HcaServer::onDisconnectResult);
        connect(w, &DisconnectWorker::disconnectResult, w, &DisconnectWorker::deleteLater);
        m_tp->push(w);
        clientsBySocket.remove(socket);
    }

    qWarning() << "Online clients: " << onlineSockets.size();
}

void HcaServer::onPingResult(QByteArray result, QWebSocket* sck)
{
    if(onlineSockets.contains(sck)) sck->sendTextMessage(result);
}

void HcaServer::onLoginResult(QByteArray result, QWebSocket* sck, QString uuid)
{
    if(onlineSockets.contains(sck)){
        sck->sendTextMessage(result);
        clientsBySocket.insert(sck, uuid);
    }
}

void HcaServer::onListWorldsResult(QByteArray result, QWebSocket* sck)
{
    if(onlineSockets.contains(sck)){ //use clientsBySocket to make sure the user is logged in
        sck->sendTextMessage(result);
    }
}

void HcaServer::onListRoomsResult(QByteArray result, QWebSocket* sck)
{
    if(onlineSockets.contains(sck)){ //use clientsBySocket to make sure the user is logged in
        sck->sendTextMessage(result);
    }
}

void HcaServer::onDisconnectResult()
{
    qWarning() << "onDisconnectResult()";
}

void HcaServer::onDbError(QString error)
{
    qWarning() << "Error connecting with database: " << error;
}

QJsonDocument HcaServer::makeErrorMessage(const QString &error)
{
    QJsonObject response;
    response[REQUEST] = ERROR;
    response[ERROR_BODY] = error;
    QJsonDocument doc;
    doc.setObject(response);
    return doc;
}
