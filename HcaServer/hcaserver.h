#ifndef HCASERVER_H
#define HCASERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "client.h"
#include "protocol.h"

class HcaServer : public QObject
{
    Q_OBJECT
public:
    explicit HcaServer(QObject *parent = 0);
    const int PORT = 8081;
    void init();

signals:

public slots:
    void onNewConnection();
    void onConnectionClosed();
    void onTextMessage(QString msg);
    void onSocketDisconnected();

private:
    QWebSocketServer *socketServer;
    QList<Client *> clients;
    QList<QWebSocket *> limbo; //list of websockets without a user
};

#endif // HCASERVER_H
