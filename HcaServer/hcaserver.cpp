#include "hcaserver.h"
#include <QElapsedTimer>
#include "pingrunnable.h"
#include "loginrunnable.h"
#include "hcarunnable.h"

HcaServer::HcaServer(QObject *parent) : QObject(parent)
{
    defaultWorld = createWorld();
    defaultWorld->setName("Hca");
    defaultWorld->setDescription("The great Hca World!");
    Room *lounge = defaultWorld->createRoom();
    lounge->setName("Lounge");
    lounge->setDescription("The public Hca Lounge");
    worlds.append(defaultWorld);
    init();
}

void HcaServer::init()
{
    qWarning() << "Main thread: " << QThread::currentThreadId();
    QThreadPool::globalInstance()->setMaxThreadCount(16);
    qWarning() << "Maximum threads: " << QThreadPool::globalInstance()->maxThreadCount();

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

Client *HcaServer::findClient(QUuid uuid)
{
    for(Client *c: clients){
        if(c->uuid() == uuid)
            return c;
    }
    return nullptr;
}

Client *HcaServer::findClient(QWebSocket *websocket)
{
    for(Client *c: clients){
        if(c->socket() == websocket)
            return c;
    }
    return nullptr;
}

Client *HcaServer::createClient()
{
    Client *c = new Client(this);
    clients.append(c);
    c->setUuid(QUuid::createUuid());
    return c;
}


World *HcaServer::findWorld(const QString &name)
{
    for(World *w : worlds){
        if(w->name() == name)
            return w;
    }
    return nullptr;
}

World *HcaServer::createWorld()
{
    return new World(this);
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

        PingRunnable *pr = new PingRunnable();
        pr->socket = socket;
        connect(pr, &PingRunnable::pingResult, this, &HcaServer::onPingResult);
        QThreadPool::globalInstance()->start(pr);
    } break;

    case LOGIN:
    {
        //Extract data from the json request
        //QUuid uuid(docObj[UUID].toString());

        /*LoginRunnable *lr  = new LoginRunnable();
        lr->socket = socket;
        lr->uuid = docObj[UUID].toString();
        connect(lr, &LoginRunnable::loginResult, this, &HcaServer::onLoginResult);
        connect(lr, &LoginRunnable::dbError, this, &HcaServer::onDbError);
        QThreadPool::globalInstance()->start(lr);
        qWarning() << "Started login runnable...";*/
        HcaRunnable *lr  = new HcaRunnable();
                //lr->socket = socket;
                //lr->uuid = docObj[UUID].toString();
                connect(lr, &HcaRunnable::loginResult, this, &HcaServer::onLoginResult);
                connect(lr, &HcaRunnable::dbError, this, &HcaServer::onDbError);
                connect(this, &HcaServer::testThread, lr, &HcaRunnable::login, Qt::QueuedConnection);
                QThreadPool::globalInstance()->start(lr);
                qWarning() << "Started login runnable...";

                emit testThread();


        /*Client *c = findClient(uuid);
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
        emit c->queueTextMessage(doc.toJson(QJsonDocument::Compact));

        qWarning() << "Online clients: " << onlineClients.size();*/
    } break;

    case LIST_WORLDS:
    {
        Client *c = findClient(socket);
        if(!c){
            socket->sendTextMessage(makeErrorMessage("Client not found").toJson(QJsonDocument::Compact));
            return;
        }

        QJsonArray worldsList;
        for(World *w : worlds){
            QJsonObject worldObj;
            worldObj[WORLD_NAME]=w->name();
            worldObj[DESCRIPTION]=w->description();
            worldObj[WORLD_SIZE]=w->size();
            worldsList.append(worldObj);
        }

        QJsonObject response;
        response[REQUEST] = LIST_WORLDS;
        response[WORLDS] = worldsList;
        QJsonDocument doc;
        doc.setObject(response);
        emit c->queueTextMessage(doc.toJson(QJsonDocument::Compact));
        qWarning() << "Client " << c->name() << " requested world list";
    } break;

    case LIST_ROOMS:
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

        QJsonArray roomsList;
        for(Room *r : w->rooms()){
            QJsonObject roomObj;
            roomObj[WORLD_NAME]=w->name();
            roomObj[ROOM_NAME]=r->name();
            roomObj[DESCRIPTION]=r->description();
            roomObj[ROOM_SIZE]=r->size();
            roomsList.append(roomObj);
        }

        QJsonObject response;
        response[REQUEST] = LIST_ROOMS;
        response[ROOMS] = roomsList;
        QJsonDocument doc;
        doc.setObject(response);
        emit c->queueTextMessage(doc.toJson(QJsonDocument::Compact));
        qWarning() << "Client " << c->name() << " requested rooms list";
    } break;

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
    } break;

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

void HcaServer::onPingResult(QByteArray result, QWebSocket* sck)
{
    sck->sendTextMessage(result);
}

void HcaServer::onLoginResult(QByteArray result, QWebSocket* sck)
{
    //limbo.removeOne(sck);
    sck->sendTextMessage(result);
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
