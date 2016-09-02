#include "loginworker.h"
#include "protocol.h"
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>


void LoginWorker::doWork(HcaThread *t)
{
    QElapsedTimer totalTime;
    totalTime.start();

    m_id = t->id();
    DbManager* dbm = t->dbManager();
    emit(t->setThreadStatus(m_id, false)); //this must always be the first

    QScopedPointer<Client> c(new Client(this));

    if(!dbm->findUserByUuid(uuid, c.data())){
        qWarning() << "User not found, creating...";
        dbm->createUser(c.data());
    }

    c->setStatus(ONLINE);
    dbm->updateClient(c.data());

    QJsonObject response = c->toFullJsonObject();
    response[REQUEST] = LOGIN;
    QJsonDocument doc;
    doc.setObject(response);
    qWarning() << "Emitting login from " << QThread::currentThreadId();
    emit loginResult(doc.toJson(QJsonDocument::Compact), socket, c->id());
    emit(t->setThreadStatus(m_id, true)); //this must always be the last
    qWarning() << "loginRequest evaded in " << totalTime.elapsed() << "ms";
}

