#ifndef LOGINWORKER_H
#define LOGINWORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QWebSocket>
#include "hcathread.h"
#include "hcaworker.h"
#include "client.h"

class LoginWorker : public HcaWorker
{
    Q_OBJECT
public:
    LoginWorker(){}
    ~LoginWorker(){}

    bool findUser();
    void createUser();

    QString uuid;
    QWebSocket* socket;
    Client client;

signals:
    void loginResult(QByteArray, QWebSocket*, int);
    void dbError(QString error);

public slots:
    void doWork(HcaThread* t);

private:
    QString name;
    QString desc;
    qint16 priv;
    qint16 status;

    QSqlDatabase m_db; //Database instance
    QString m_id; //Thread id
};

#endif // LOGINWORKER_H
