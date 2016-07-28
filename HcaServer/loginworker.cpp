#include "loginworker.h"
#include "protocol.h"
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>

bool LoginWorker::findUser()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM clients WHERE uuid = :uuid");
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
        QSqlQuery updQuery(m_db);
        updQuery.prepare("UPDATE clients SET status = :st WHERE uuid = :uuid");
        updQuery.bindValue(":st", ONLINE);
        updQuery.bindValue(":uuid", uuid);
        if(!updQuery.exec()){
            emit dbError(updQuery.lastError().text());
            return false;
        } return true; //user found and updated
    } else return false; //user not found
}

void LoginWorker::createUser()
{
    qWarning() << "User NOT found, creating it in new Thread";
    name = QStringLiteral("unnamed");
    desc = QStringLiteral("");
    priv = USER;
    status = ONLINE;
    uuid = QUuid::createUuid().toString();

    QSqlQuery insQuery(m_db);
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

void LoginWorker::doWork(HcaThread *t)
{
    QElapsedTimer totalTime;
    totalTime.start();

    m_id = t->id();
    m_db = t->db();
    emit(t->setThreadStatus(m_id, false)); //this must always be the first


    if(!findUser()){
        createUser();
    }

    QJsonObject response;
    response[REQUEST] = LOGIN;
    response[NAME] = name;
    response[UUID] = uuid;
    QJsonDocument doc;
    doc.setObject(response);
    qWarning() << "Emitting login from " << QThread::currentThreadId();
    emit loginResult(doc.toJson(QJsonDocument::Compact), socket, uuid);
    emit(t->setThreadStatus(m_id, true)); //this must always be the last
    qWarning() << "loginRequest evaded in " << totalTime.elapsed() << "ms";
}

