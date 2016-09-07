#include "hcaserver.h"
#include <QElapsedTimer>
#include "pingrunnable.h"
#include "loginworker.h"
#include "disconnectworker.h"
#include "listworldsworker.h"
#include "listroomsworker.h"
#include "joinroomworker.h"
#include "createroomworker.h"
#include "leaveroomworker.h"

HcaServer::HcaServer(QObject *parent) : QObject(parent)
{
    init();
}

void HcaServer::init()
{
    qWarning() << "Main Thread: " << QThread::currentThreadId();
    m_tp = new HcaThreadPool(this);

    if (!QSqlDatabase::drivers().contains("QPSQL")){
        qWarning() << "Unable to load database, PSQL driver missing";
        return;
    } else {
        qWarning() << "PSQL driver found, you are good to go!";
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
        //PARAMS:
        // uuid

        QPointer<LoginWorker> w = new LoginWorker();
        w->uuid = docObj[UUID].toString();
        w->socket = socket;
        connect(w, &LoginWorker::loginResult, this, &HcaServer::onLoginResult);
        connect(w, &LoginWorker::loginResult, w, &LoginWorker::deleteLater);
        m_tp->push(w);
    } break;

    case LIST_WORLDS:
    {
        //PARAMS:
        // -
        QPointer<ListWorldsWorker> w = new ListWorldsWorker();
        w->socket = socket;
        connect(w, &ListWorldsWorker::listWorldsResult, this, &HcaServer::onListWorldsResult);
        connect(w, &ListWorldsWorker::listWorldsResult, w, &ListWorldsWorker::deleteLater);
        m_tp->push(w);
    } break;

    case LIST_ROOMS:
    {
        //PARAMS:
        // world id
        qint32 worldId = docObj[WORLD_ID].toInt();
        QPointer<ListRoomsWorker> w(new ListRoomsWorker());
        w->socket = socket;
        w->worldId = worldId;
        connect(w, &ListRoomsWorker::listRoomsResult, this, &HcaServer::onListRoomsResult);
        connect(w, &ListRoomsWorker::listRoomsResult, w, &ListRoomsWorker::deleteLater);
        m_tp->push(w);
    } break;

    case JOIN_ROOM:
    {
        //PARAMS:
        // room id
        int roomId = docObj[ROOM_ID].toInt();

        if(!clientsBySocket.contains(socket)){
            qWarning() << "ERROR: join room from non logged in user";
            break;
        }

        int clientId = clientsBySocket.value(socket);

        QPointer<JoinRoomWorker> w = new JoinRoomWorker ();
        w->socket = socket;
        w->roomId = roomId;
        w->clientId = clientId;
        connect(w, &JoinRoomWorker::joinRoomResult, this, &HcaServer::onJoinRoomResult);
        connect(w, &JoinRoomWorker::joinRoomResult, w, &JoinRoomWorker::deleteLater);
        m_tp->push(w);
    } break;

    case CREATE_ROOM:
    {
        //PARAMS:
        // world id
        // room name
        // room description
        // room motd
        // room avatar

        int worldId = docObj[WORLD_ID].toInt();
        QString roomName = docObj[ROOM_NAME].toString();
        QString description = docObj[DESCRIPTION].toString();
        QString motd = docObj[ROOM_MOTD].toString();
        QString avatar = docObj[ROOM_AVATAR].toString();

        if(!clientsBySocket.contains(socket)){
            qWarning() << "ERROR: create room from non logged in user";
            break;
        }

        int clientId = clientsBySocket.value(socket);

        QPointer<CreateRoomWorker> w = new CreateRoomWorker();
        w->ownerId = clientId;
        w->name = roomName;
        w->description = description;
        w->motd = motd;
        w->worldId = worldId;
        w->avatar = avatar;
        connect(w, &CreateRoomWorker::createRoomResult, this, &HcaServer::onCreateRoomResult);
        connect(w, &CreateRoomWorker::createRoomResult, w, &CreateRoomWorker::deleteLater);
        m_tp->push(w);
    } break;

    case LEAVE_ROOM:
    {
        //PARAMS:
        // room id
        int roomId = docObj[ROOM_ID].toInt();

        if(!clientsBySocket.contains(socket)){
            qWarning() << "ERROR: leave room from non logged in user";
            break;
        }

        int clientId = clientsBySocket.value(socket);

        QPointer<LeaveRoomWorker> w = new LeaveRoomWorker();
        w->clientId = clientId;
        w->roomdId = roomId;
        connect(w, &LeaveRoomWorker::leaveRoomResult, this, &HcaServer::onLeaveRoomResult);
        connect(w, &LeaveRoomWorker::leaveRoomResult, w, &LeaveRoomWorker::deleteLater);
        m_tp->push(w);

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
    onlineSockets.removeOne(socket); //even if it's not there

    //if there's a client with that socket, set it as "offline"
    if(clientsBySocket.contains(socket)){
        QPointer<DisconnectWorker> w = new DisconnectWorker();
        w->id = clientsBySocket.value(socket);
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

void HcaServer::onLoginResult(QByteArray result, QWebSocket* sck, int id)
{
    if(onlineSockets.contains(sck)){
        sck->sendTextMessage(result);
        clientsBySocket.insert(sck, id);
    }
}

void HcaServer::onListWorldsResult(QByteArray result, QWebSocket* sck)
{
    if(clientsBySocket.contains(sck)){ //use clientsBySocket to make sure the user is logged in
        sck->sendTextMessage(result);
    }
}

void HcaServer::onListRoomsResult(QByteArray result, QWebSocket* sck)
{
    if(clientsBySocket.contains(sck)){ //use clientsBySocket to make sure the user is logged in
        sck->sendTextMessage(result);
    }
}

void HcaServer::onJoinRoomResult(QByteArray result, QWebSocket *sck)
{
    if(clientsBySocket.contains(sck)){
        sck->sendTextMessage(result);
    }
}

void HcaServer::onCreateRoomResult(QByteArray result, QWebSocket *sck)
{
    if(clientsBySocket.contains(sck)){
        sck->sendTextMessage(result);
    }
}

void HcaServer::onLeaveRoomResult(QByteArray result, QWebSocket *sck)
{
    if(clientsBySocket.contains(sck)){
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
