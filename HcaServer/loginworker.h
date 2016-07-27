#ifndef LOGINWORKER_H
#define LOGINWORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QWebSocket>
#include "hcathread.h"
#include "hcaworker.h"

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

signals:
    void loginResult(QByteArray, QWebSocket*);
    void dbError(QString error);

public slots:
    void doWork(HcaThread* t);

private:
    QString name;
    QString desc;
    qint16 priv;
    qint16 status;

    QSqlDatabase m_db;
    QString m_id;
};

#endif // LOGINWORKER_H
