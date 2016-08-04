#ifndef HCASERVER_H
#define HCASERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include "client.h"
#include "room.h"
#include "world.h"
#include "protocol.h"
#include "hcathread.h"
#include "hcathreadpool.h"

class HcaServer : public QObject
{
    Q_OBJECT
public:
    explicit HcaServer(QObject *parent = 0);
    const int PORT = 8081;

    void init();

    QJsonDocument makeErrorMessage(const QString &error);

signals:
    void sendTextMessage(const QString &message);

public slots:
    void onNewConnection();
    void onConnectionClosed();
    void onTextMessage(QString msg);
    void onSocketDisconnected();

    void onPingResult(QByteArray, QWebSocket*);
    void onLoginResult(QByteArray, QWebSocket*, int);
    void onListWorldsResult(QByteArray, QWebSocket*);
    void onListRoomsResult(QByteArray, QWebSocket*);
    void onJoinRoomResult(QByteArray, QWebSocket*);
    void onCreateRoomResult(QByteArray, QWebSocket*);
    void onLeaveRoomResult(QByteArray, QWebSocket*);
    void onDisconnectResult();

    void onDbError(QString error);

private:
    HcaThreadPool *m_tp;
    QList<QWebSocket *> onlineSockets; //list of active sockets

    //store the relation between client ids and sockets
    QHash<QWebSocket*, int> clientsBySocket;
    QWebSocketServer *socketServer;
};

#endif // HCASERVER_H
