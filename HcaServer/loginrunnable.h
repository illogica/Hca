#ifndef LOGINRUNNABLE_H
#define LOGINRUNNABLE_H

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
#include "client.h"
#include "protocol.h"

class LoginRunnable : public QObject, public QRunnable
{
    Q_OBJECT

private:
    QSqlDatabase db;

    QString name;
    QString desc;
    qint16 priv;
    qint16 status;

public:
    QWebSocket *socket;
    QString uuid;

    void run(){

        if(!connect()) return;

        if(!findUser()) createUser();

        QJsonObject response;
        response[REQUEST] = LOGIN;
        response[NAME] = name;
        response[UUID] = uuid;
        QJsonDocument doc;
        doc.setObject(response);
        qWarning() << "Emitting login from " << QThread::currentThreadId();
        emit loginResult(doc.toJson(QJsonDocument::Compact), socket);
        db.close();
    }

    bool connect(){
        db = QSqlDatabase::addDatabase("QPSQL", QString::number((qintptr)QThread::currentThreadId()));
        db.setHostName("127.0.0.1");
        db.setDatabaseName("hca");
        db.setUserName("hca");
        //db.setPassword("");
        if(!db.open()){
            qWarning() << "Error opening database: " << db.lastError().text();
            emit dbError(db.lastError().text());
            return false;
        } else {
            qWarning() << "Database open";
            return true;
        }
    }

    bool findUser(){
        QSqlQuery query(db);
        query.prepare("SELECT * FROM clients WHERE uuid = ':uuid'");
        query.bindValue(":uuid", uuid);
        if(!query.exec()){
            qWarning() << "Error in " << query.lastQuery();
            emit dbError(query.lastError().text());
            return false;
        }

        if(query.next()){
            qWarning() << "User found, updating state in new Thread";
            name = query.value(1).toString();
            desc = query.value(2).toString();
            priv = query.value(3).toInt();
            status = query.value(4).toInt();
            QSqlQuery updQuery(db);
            updQuery.prepare("UPDATE clients SET status = :st WHERE uuid = :uuid");
            updQuery.bindValue(":status", ONLINE);
            updQuery.bindValue(":uuid", uuid);
            if(!updQuery.exec()){
                emit dbError(updQuery.lastError().text());
                return false;
            } return true; //user found and updated
        } else return false; //user not found
    }

    void createUser(){
        qWarning() << "User NOT found, creating it in new Thread";
        name = QStringLiteral("unnamed");
        desc = QStringLiteral("");
        priv = USER;
        status = ONLINE;
        uuid = QUuid::createUuid().toString();

        QSqlQuery insQuery(db);
        insQuery.prepare("INSERT INTO clients(uuid, name, description, privilege, status) VALUES (:uuid, :name, :description, :privilege, :status)");
        insQuery.bindValue(0, uuid);
        insQuery.bindValue(1, name);
        insQuery.bindValue(2, desc);
        insQuery.bindValue(3, priv);
        insQuery.bindValue(4, status);
        if(!insQuery.exec()){
            qWarning() << "Error in " << insQuery.lastQuery();
            emit dbError(insQuery.lastError().text());
            return;
        } else {
            qWarning() << "User created";
        }
    }

signals:
    void loginResult(QByteArray, QWebSocket*);
    void dbError(QString error);
};

#endif // LOGINRUNNABLE_H
