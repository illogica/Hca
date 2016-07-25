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

class HcaServer : public QObject
{
    Q_OBJECT
public:
    explicit HcaServer(QObject *parent = 0);
    const int PORT = 8081;

    void init();

    Client *findClient(QUuid uuid);
    Client *findClient(QWebSocket *websocket);
    Client *createClient();

    World *findWorld(const QString &name);
    World *createWorld();

    QJsonDocument makeErrorMessage(const QString &error);

signals:
    void sendTextMessage(const QString &message);

public slots:
    void onNewConnection();
    void onConnectionClosed();
    void onTextMessage(QString msg);
    void onSocketDisconnected();

private:
    int m_maxThreads;
    QList<HcaThread *> m_threadPool;
    QList<Client *> clients; //to be put to REST

    QWebSocketServer *socketServer;
    QList<Client *> onlineClients;
    QList<QWebSocket *> limbo; //list of websockets without a user

    QList<World *> worlds; //fundamental type. At least the default world always exists
    World *defaultWorld;
};

#endif // HCASERVER_H
