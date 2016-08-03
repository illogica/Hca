#include "dbmanager.h"
#include "protocol.h"
#include <QUuid>

DbManager::DbManager()
{}

void DbManager::init(QSqlDatabase db)
{
    m_db = db;
}

bool DbManager::findUserByUuid(const QString &uuid, Client* c)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM clients WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);
    if(!query.exec()){
        qWarning() << "Error in " << query.lastQuery();
        qWarning() << "ERROR: " << query.lastError().text();
        return false;
    }
    if(query.next()){
        c->setId(query.value(0).toInt());
        c->setUuid(query.value(1).toString());
        c->setName(query.value(2).toString());
        c->setDescription(query.value(3).toString());
        c->setPrivilege(query.value(4).toInt());
        c->setStatus(query.value(5).toInt());
        c->setAvatar(query.value(6).toString());
        qWarning() << "User found " << c->id();
        return true;
    } return false;
}

bool DbManager::findUserById(int id, Client* c)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM clients WHERE id = :id");
    query.bindValue(":id", id);
    if(!query.exec()){
        qWarning() << "Error in " << query.lastQuery();
        qWarning() << "ERROR: " << query.lastError().text();
        return false;
    }
    if(query.next()){
        c->setId(query.value(0).toInt());
        c->setUuid(query.value(1).toString());
        c->setName(query.value(2).toString());
        c->setDescription(query.value(3).toString());
        c->setPrivilege(query.value(4).toInt());
        c->setStatus(query.value(5).toInt());
        c->setAvatar(query.value(6).toString());
        qWarning() << "User found " << c->id();
        return true;
    } return false;
}

void DbManager::createUser(Client *c)
{
    c->setName(QStringLiteral("unnamed"));
    c->setDescription(QStringLiteral(""));
    c->setPrivilege(USER);
    c->setStatus(ONLINE);
    c->setUuid(QUuid::createUuid().toString());
    c->setAvatar(QStringLiteral("1f42a"));

    QSqlQuery insQuery(m_db);
    insQuery.prepare("INSERT INTO clients(uuid, name, description, privilege, status, avatar) VALUES (:uuid, :name, :description, :privilege, :status, :avatar) RETURNING id");
    insQuery.bindValue(":uuid", c->uuid());
    insQuery.bindValue(":name", c->name());
    insQuery.bindValue(":description", c->description());
    insQuery.bindValue(":privilege", c->privilege());
    insQuery.bindValue(":status", c->status());
    insQuery.bindValue(":avatar", c->avatar());
    if(!insQuery.exec()){
        qWarning() << "Error in " << insQuery.lastQuery();
        qWarning() << "ERROR: " << insQuery.lastError().text();
    } else {
        insQuery.next();
        c->setId(insQuery.value(0).toInt());
        qWarning() << "User created";
    }
}

void DbManager::updateClientStatus(int id, int status)
{
    QSqlQuery updQuery(m_db);
    updQuery.prepare("UPDATE clients SET status = :st WHERE id = :id");
    updQuery.bindValue(":st", status);
    updQuery.bindValue(":id", id);
    if(!updQuery.exec()){
        qWarning() << "ERROR: " << updQuery.lastError().text();
    } //user found and updated
}

void DbManager::updateClient(Client *c)
{
    QSqlQuery updQuery(m_db);
    updQuery.prepare("UPDATE clients SET uuid = :uuid, name = :name, description = :description, privilege = :privilege, status = :status, avatar = :avatar WHERE id = :id");
    updQuery.bindValue(":uuid", c->uuid());
    updQuery.bindValue(":name", c->name());
    updQuery.bindValue(":description", c->description());
    updQuery.bindValue(":privilege", c->privilege());
    updQuery.bindValue(":status", c->status());
    updQuery.bindValue(":avatar", c->avatar());

    updQuery.bindValue(":id", c->id());
    if(!updQuery.exec()){
        qWarning() << "ERROR: " << updQuery.lastError().text();
    } //user found and updated
}
