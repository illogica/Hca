#ifndef HCATHREAD_H
#define HCATHREAD_H

#include <QObject>
#include <QThread>
#include <QtSql>
#include <dbmanager.h>

class HcaServer;

class HcaThread : public QThread
{
    Q_OBJECT
public:
    HcaThread(){}
    ~HcaThread(){}
    void run() Q_DECL_OVERRIDE;

    QSqlDatabase db() const;

    QString id() const;

    DbManager* dbManager() const;

signals:
    void initialized(HcaThread*);
    void setThreadStatus(QString t_id, bool available);

private:
    QSqlDatabase m_db;
    QString m_id;
    DbManager *m_dbManager;
};

Q_DECLARE_METATYPE(HcaThread*)
#endif // HCATHREAD_H
