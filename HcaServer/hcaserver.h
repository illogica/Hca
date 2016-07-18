#ifndef HCASERVER_H
#define HCASERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>
#include "client.h"
#include "protocol.h"

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

signals:

public slots:
    void onNewConnection();
    void onConnectionClosed();
    void onTextMessage(QString msg);
    void onSocketDisconnected();

private:
    QList<Client *> clients; //to be put to REST

    QWebSocketServer *socketServer;
    QList<Client *> onlineClients;
    QList<QWebSocket *> limbo; //list of websockets without a user
};

#endif // HCASERVER_H
