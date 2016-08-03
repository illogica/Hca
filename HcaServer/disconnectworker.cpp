#include "disconnectworker.h"
#include "protocol.h"

void DisconnectWorker::doWork(HcaThread *t)
{
    m_id = t->id();
    DbManager* dbm = t->dbManager();
    emit(t->setThreadStatus(m_id, false)); //this must always be the first

    QScopedPointer<Client> c(new Client(this));

    if(!dbm->findUserById(id, c.data())){
        qWarning() << "User not found, cannot update its state";
        return;
    }

    c->setStatus(OFFLINE);
    dbm->updateClient(c.data());
    //user found and updated

    qWarning() << "Emitting disconnect from " << QThread::currentThreadId();
    emit disconnectResult();
    emit(t->setThreadStatus(m_id, true)); //this must always be the last
}
