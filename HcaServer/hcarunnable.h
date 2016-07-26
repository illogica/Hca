#ifndef HCARUNNABLE_H
#define HCARUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QUuid>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QEventLoop>
#include "client.h"
#include "protocol.h"

class HcaRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    HcaRunnable();

    QString threadId;

    QString uuid;

    void run();
    bool connect();
    bool findUser();
    void createUser();

public slots:
    void login();

signals:
    void loginResult(QByteArray, QWebSocket*);
    void dbError(QString error);

private:
    QSqlDatabase db;
    QEventLoop *m_el;
    QWebSocket *m_socket;

    QString name;
    QString desc;
    qint16 priv;
    qint16 status;

};

#endif // HCARUNNABLE_H
