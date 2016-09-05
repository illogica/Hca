#include "hcaclient.h"
#include "QUuid"
#include "../HcaServer/protocol.h"

HcaClient::HcaClient(QObject *parent, QQmlContext *context) : QObject(parent)
{
    m_active = false;
    m_connected = false;
    m_getWorldsListPending = false;
    m_getRoomsListPending = false;
    m_loginPending = false;

    this->ctx = context;
    connect(&socket, &QWebSocket::connected, this, &HcaClient::onConnected);
    connect(&socket, &QWebSocket::disconnected, this, &HcaClient::onDisconnected);
    connect(&socket, &QWebSocket::textMessageReceived, this, &HcaClient::parseServerMessage);

    ctx->setContextProperty("wlModel", QVariant::fromValue(m_worldsModel));
    connectionTimer = new QTimer(this);
    connect(connectionTimer, &QTimer::timeout, this, &HcaClient::establish);
}

void HcaClient::establish(){
    if(!m_connected){
        qWarning() << "trying to connect";
        socket.open(QUrl(QStringLiteral("ws://localhost:8081")));
    } else {
        qWarning() << "Called establish for nothing!";
    }
 }

void HcaClient::onConnected()
{
    qWarning() << "onConnected()";
    m_connected = true;
    emit connectedChanged(true);
    //socket.sendTextMessage(makePing().toJson(QJsonDocument::Compact));
    sendLogin();
}

void HcaClient::onDisconnected()
{
    qWarning() << "onDisconnected()";
    m_connected = false;
    emit connectedChanged(false);
    if(m_active){
        QTimer::singleShot(RETRY_MS, this, SLOT(establish()));
    }
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
    } break;

    case PONG:
    {
        qWarning() << "PONG";
    } break;

    case LOGIN:
    {
        QUuid uuid(docObj[UUID].toString());
        settings.setValue(UUID, uuid);
        settings.setValue(NAME, docObj[NAME].toString());
        qWarning() << "new Uuid: " << uuid.toString();
        m_loginPending = false;
        sendGetWorldsList();
    } break;

    case JOIN_ROOM:
    {
        QString name = docObj[NAME].toString();
        qWarning() << "Joined room: " << name;
    } break;

    case LIST_WORLDS:
    {
        m_getWorldsListPending = false;
        emit qmlWorldsListReset();
        QJsonArray worlds = docObj[WORLDS].toArray();

        QJsonArray::iterator it;
        for(it = worlds.begin(); it!=worlds.end(); it++){
            QJsonObject obj = (*it).toObject();
            qmlWorldsListAdd(
                        obj[WORLD_ID].toInt(),
                        obj[WORLD_NAME].toString(),
                        obj[DESCRIPTION].toString(),
                        obj[WORLD_SIZE].toInt());
        }

        qWarning() << "Updated worlds lists";
    } break;

    case LIST_ROOMS:
    {
        m_getRoomsListPending = false;
        qint32 worldId = docObj[WORLD_ID].toInt();
        WorldData *w = findWorld(worldId);
        if(!w) return;

        QJsonArray rooms = docObj[ROOMS].toArray();
        if(rooms.isEmpty()) return;

        //we have enough data, let's start:
        w->resetRooms();

        QJsonArray::iterator it;
        for(it = rooms.begin(); it!=rooms.end(); it++){
            QJsonObject obj = (*it).toObject();
            RoomData *r = new RoomData(this);
            r->setName(obj[ROOM_NAME].toString());
            r->setDescription(obj[DESCRIPTION].toString());
            r->setSize(obj[ROOM_SIZE].toInt());
            w->addRoom(r);
        }

        //Share the data with the QML side
        ctx->setContextProperty("roomsModel", QVariant::fromValue(w->roomsModel()));

        qWarning() << "Updated rooms list for " << w->name();
    } break;

    default:
        qWarning() << "unrecognized command";
    }
}

void HcaClient::joinRoom(int roomId)
{
    /*QString rName = roomName.trimmed();
    if(rName.length() < 3 || rName.length()>32) return;
    QString wName = worldName.trimmed();
    if(wName.length() < 3 || wName.length()>32) return;

    QJsonObject request;
    request[REQUEST] = JOIN_ROOM;
    request[WORLD_NAME] = wName;
    request[ROOM_NAME] = rName;
    QJsonDocument doc;
    doc.setObject(request);
    socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));*/
    QJsonObject request;
    request[REQUEST] = JOIN_ROOM;
    request[ROOM_ID] = roomId;
    QJsonDocument doc;
    doc.setObject(request);
    socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
}

void HcaClient::createRoom(const QString name, const QString description, const QString motd)
{
    Q_UNUSED(name);
    Q_UNUSED(description);
    Q_UNUSED(motd);
}

void HcaClient::leaveRoom(int roomId)
{
    QJsonObject request;
    request[REQUEST] = LEAVE_ROOM;
    request[ROOM_ID] = roomId;
    QJsonDocument doc;
    doc.setObject(request);
    socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
}

void HcaClient::sendLogin()
{
    if(m_connected && !m_loginPending){
        QJsonObject response;
        response[REQUEST] = LOGIN;
        response[UUID] = settings.value(UUID, "").toUuid().toString();
        QJsonDocument doc;
        doc.setObject(response);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
        m_loginPending = true;
    }
}

void HcaClient::sendPing()
{
    if(m_connected){
        QJsonObject response;
        response[REQUEST] = PING;
        QJsonDocument doc;
        doc.setObject(response);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void HcaClient::sendGetRoomsList(qint32 worldId)
{
    if(m_connected && !m_getRoomsListPending){
        QJsonObject request;
        request[REQUEST] = LIST_ROOMS;
        request[WORLD_ID] = worldId;
        QJsonDocument doc;
        doc.setObject(request);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
        m_getRoomsListPending = true;
    }
}

WorldData *HcaClient::findWorld(int worldId)
{
    for(WorldData *w : m_worlds){
        if(w->id()==worldId)
            return w;
    }
    return nullptr;
}

void HcaClient::sendGetWorldsList()
{
    if(m_connected && !m_getWorldsListPending){
        QJsonObject request;
        request[REQUEST] = LIST_WORLDS;
        QJsonDocument doc;
        doc.setObject(request);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
        m_getWorldsListPending = true;
    }
}

void HcaClient::sendJoinRoom(int roomId)
{
    if(m_connected){
        QJsonObject request;
        request[REQUEST] = JOIN_ROOM;
        request[ROOM_ID] = roomId;
        QJsonDocument doc;
        doc.setObject(request);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void HcaClient::sendCreateRoom(const QString &name, const QString &description, const QString &motd, const QString &avatar)
{
    if(m_connected){
        QJsonObject request;
        request[REQUEST] = CREATE_ROOM;
        request[ROOM_NAME] = name;
        request[DESCRIPTION] = description;
        request[ROOM_MOTD] = motd;
        request[ROOM_AVATAR] = avatar;
        QJsonDocument doc;
        doc.setObject(request);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void HcaClient::sendLeaveRoom(int roomId)
{
    if(m_connected){
        QJsonObject request;
        request[REQUEST] = LEAVE_ROOM;
        request[ROOM_ID] = roomId;
        QJsonDocument doc;
        doc.setObject(request);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

bool HcaClient::connected(){ return m_connected;}

bool HcaClient::active()
{
    return m_active;
}

void HcaClient::setActive(bool active)
{
    if(m_active != active){
        m_active = active;
        emit activeChanged(active);

        if(active){
            establish();
        }
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
