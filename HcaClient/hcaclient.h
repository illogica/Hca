#ifndef HCACLIENT_H
#define HCACLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>

class HcaClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)
public:
    explicit HcaClient(QObject *parent = 0);
    void establish();

signals:
    void connectedChanged(bool connected);

public slots:
    void onConnected();
    void onDisconnected();
    void parseServerMessage(const QString &message);

    //
    void sendLogin();

    //Properties stuff
    bool connected();
    void setConnected(bool cntd);


private:
    QJsonDocument makePing();

    QSettings settings;
    QWebSocket socket;
    bool m_connected;
};

#endif // HCACLIENT_H