#include "loginworker.h"
#include "protocol.h"
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>

bool LoginWorker::findUser()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM clients WHERE uuid = :uuid");
    query.bindValue(":uuid", client.uuid());
    if(!query.exec()){
        qWarning() << "Error in " << query.lastQuery();
        emit dbError(query.lastError().text());
        return false;
    }

    if(query.next()){
        qWarning() << "User found, updating state in new Thread";
        client.setId(query.value(0).toInt());
        client.setName(query.value(1).toString());
        client.setUuid(query.value(2).toString());
        client.setDescription(query.value(3).toString());
        client.setPrivilege(query.value(4).toInt());
        client.setStatus(query.value(5).toInt());
        client.setAvatar(query.value(6).toString());
        QSqlQuery updQuery(m_db);
        updQuery.prepare("UPDATE clients SET status = :st WHERE id = :id");
        updQuery.bindValue(":st", ONLINE);
        updQuery.bindValue(":id", client.id());
        if(!updQuery.exec()){
            emit dbError(updQuery.lastError().text());
            return false;
        } return true; //user found and updated
    } else return false; //user not found
}

void LoginWorker::createUser()
{
    qWarning() << "User NOT found, creating it in new Thread";
    client.setName(QStringLiteral("unnamed"));
    client.setDescription(QStringLiteral(""));
    client.setPrivilege(USER);
    client.setStatus(ONLINE);
    client.setUuid(QUuid::createUuid().toString());
    client.setAvatar(QStringLiteral("1f42a"));

    QSqlQuery insQuery(m_db);
    insQuery.prepare("INSERT INTO clients(uuid, name, description, privilege, status, avatar) VALUES (:uuid, :name, :description, :privilege, :status, :avatar) RETURNING id");
    insQuery.bindValue(0, client.uuid());
    insQuery.bindValue(1, client.name());
    insQuery.bindValue(2, client.description());
    insQuery.bindValue(3, client.privilege());
    insQuery.bindValue(4, client.status());
    insQuery.bindValue(5, client.avatar());
    if(!insQuery.exec()){
        qWarning() << "Error in " << insQuery.lastQuery();
        emit dbError(insQuery.lastError().text());
        return;
    } else {
        insQuery.next();
        client.setId(insQuery.value(0).toInt());
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

    QJsonObject response = client.toFullJsonObject();
    response[REQUEST] = LOGIN;
    QJsonDocument doc;
    doc.setObject(response);
    qWarning() << "Emitting login from " << QThread::currentThreadId();
    emit loginResult(doc.toJson(QJsonDocument::Compact), socket, client.id());

    emit(t->setThreadStatus(m_id, true)); //this must always be the last
    qWarning() << "loginRequest evaded in " << totalTime.elapsed() << "ms";
}

