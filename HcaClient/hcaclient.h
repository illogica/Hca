#ifndef HCACLIENT_H
#define HCACLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSettings>
#include <QQmlContext>
#include "roomdata.h"
#include "worlddata.h"

class HcaClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)
public:
    explicit HcaClient(QObject *parent = 0, QQmlContext *context = 0);
    void establish();

signals:
    void connectedChanged(bool connected);

public slots:
    void onConnected();
    void onDisconnected();
    void parseServerMessage(const QString &message);
    void joinRoom(const QString &roomName, const QString &worldName);
    void leaveRoom(const QString &roomName, const QString &worldName);

    //communication
    void sendLogin();
    void sendGetWorldsList();
    void sendGetRoomsList(const QString &worldName);

    //utils
    WorldData* findWorld(const QString &name);

    //Properties stuff
    bool connected();
    void setConnected(bool cntd);


private:
    QJsonDocument makePing();

    QSettings settings;
    QWebSocket socket;
    bool m_connected;
    QQmlContext *ctx;

    WorldData *currentWorld;
    QList<WorldData *> m_worlds;
    QList<QObject *> m_worldsModel;
};

#endif // HCACLIENT_H
