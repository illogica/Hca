#include "hcaclient.h"
#include "QUuid"
#include "../HcaServer/protocol.h"

HcaClient::HcaClient(QObject *parent, QQmlContext *context) : QObject(parent)
{
    this->ctx = context;
    connect(&socket, &QWebSocket::connected, this, &HcaClient::onConnected);
    connect(&socket, &QWebSocket::disconnected, this, &HcaClient::onDisconnected);
    connect(&socket, &QWebSocket::textMessageReceived, this, &HcaClient::parseServerMessage);

    ctx->setContextProperty("wlModel", QVariant::fromValue(m_worldsModel));
}

void HcaClient::establish(){
    socket.open(QUrl(QStringLiteral("ws://localhost:8081")));
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
        sendGetWorldsList();
    } break;

    case JOIN_ROOM:
    {
        QString name = docObj[NAME].toString();
        qWarning() << "Joined room: " << name;
    } break;

    case LIST_WORLDS:
    {
        QJsonArray worlds = docObj[WORLDS].toArray();

        //update the worlds we already have
        QJsonArray::iterator it;
        for(it = worlds.begin(); it!=worlds.end(); it++){
            QJsonObject obj = (*it).toObject();
            WorldData *w = findWorld(obj[WORLD_ID].toInt());
            if(!w){ w = new WorldData(this); m_worlds.append(w);}
            w->setId(obj[WORLD_ID].toInt());
            w->setName(obj[WORLD_NAME].toString());
            w->setDescription(obj[DESCRIPTION].toString());
            w->setSize(obj[WORLD_SIZE].toInt());
        }

        //delete worlds that don't exist anymore
        for(WorldData *w : m_worlds){
            bool found = false;
            for(it = worlds.begin(); it!=worlds.end(); it++){
                QJsonObject obj = (*it).toObject();
                if(obj[WORLD_ID].toInt() == w->id()){
                    found = true;
                    break;
                }
            }
            if(!found) delete w;
        }

        //Share the data with the QML side
        qDeleteAll(m_worldsModel.begin(), m_worldsModel.end());
        for(WorldData *w : m_worlds){
            m_worldsModel.append(new WorldData(w->id(), w->name(), w->description(), w->size()));
        }
        ctx->setContextProperty("wlModel", QVariant::fromValue(m_worldsModel));

        qWarning() << "Updated worlds lists";
    } break;

    case LIST_ROOMS:
    {
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
    if(m_connected){
        QJsonObject response;
        response[REQUEST] = LOGIN;
        response[UUID] = settings.value(UUID, "").toUuid().toString();
        QJsonDocument doc;
        doc.setObject(response);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
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
    if(m_connected){
        QJsonObject request;
        request[REQUEST] = LIST_ROOMS;
        request[WORLD_ID] = worldId;
        QJsonDocument doc;
        doc.setObject(request);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
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
    if(m_connected){
        QJsonObject request;
        request[REQUEST] = LIST_WORLDS;
        QJsonDocument doc;
        doc.setObject(request);
        socket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
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
    response[REQUEST] = PONG;
    QJsonDocument doc;
    doc.setObject(response);
    return doc;
}
