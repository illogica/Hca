#ifndef HCASERVER_H
#define HCASERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QDebug>
#include "client.h"

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
    QList<QWebSocket *> sockets;
};

#endif // HCASERVER_H
