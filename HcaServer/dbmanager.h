#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtSql>
#include <QThread>
#include "client.h"

class DbManager : public QObject
{
    Q_OBJECT
public:
    DbManager();
    ~DbManager(){}
    void init(QSqlDatabase db);
    bool findUserByUuid(const QString &uuid, Client* c);
    bool findUserById(int id, Client* c);
    void createUser(Client* c);
    void updateClientStatus(int id, int status);
    void updateClient(Client* c);

signals:

public slots:

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
