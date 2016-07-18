#ifndef HCACLIENT_H
#define HCACLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>

class HcaClient : public QObject
{
    Q_OBJECT
public:
    explicit HcaClient(QObject *parent = 0);

signals:

public slots:
    void onConnected();
    void onDisconnected();
    void parseServerMessage(QString &message);

private:
    QJsonDocument makePing();

    QWebSocket socket;
};

#endif // HCACLIENT_H
