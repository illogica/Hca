#ifndef HCACLIENT_H
#define HCACLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSettings>
#include <QQmlContext>
#include <QTimer>
#include "roomdata.h"
#include "worlddata.h"

#define RETRY_MS 3000

class HcaClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
public:
    explicit HcaClient(QObject *parent = 0, QQmlContext *context = 0);

signals:
    void connectedChanged(bool connected);
    void activeChanged(bool active);

    void qmlWorldsListAdd(int id, const QString &name, const QString &description, int size);
    void qmlWorldsListReset();
    void qmlRoomsListAdd(int id, const QString &name, const QString &description, const QString &motd, int size, int count, const QString &avatar);
    void qmlRoomsListReset();

public slots:
    void onConnected();
    void onDisconnected();
    void establish();
    void parseServerMessage(const QString &message);
    void joinRoom(int roomId);
    void createRoom(const QString name, const QString description, const QString motd);
    void leaveRoom(int roomId);

    //communication
    void sendLogin();
    void sendPing();
    void sendGetWorldsList();
    void sendGetRoomsList(int worldId);
    void sendJoinRoom(int roomId);
    void sendCreateRoom(const QString &name, const QString &description, const QString &motd, const QString &avatar);
    void sendLeaveRoom(int roomId);

    //utils
    WorldData* findWorld(int worldId);

    //Properties stuff
    bool connected();
    bool active();
    void setActive(bool active);

private:
    QJsonDocument makePing();
    bool m_getWorldsListPending;
    bool m_getRoomsListPending;
    bool m_loginPending;

    QTimer *connectionTimer;

    QSettings settings;
    QWebSocket socket;
    bool m_connected;
    bool m_active;
    QQmlContext *ctx;

    WorldData *currentWorld;
    QList<WorldData *> m_worlds;
    QList<QObject *> m_worldsModel;
};

#endif // HCACLIENT_H
