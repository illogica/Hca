#ifndef HCATHREAD_H
#define HCATHREAD_H

#include <QObject>
#include <QThread>
#include <QtSql>

class HcaServer;

class HcaThread : public QThread
{
    Q_OBJECT
public:
    HcaThread(int id, HcaServer* srv);
    void run() Q_DECL_OVERRIDE;

public slots:
    void testText();

signals:
    void initialized(int id);

private:
    QSqlDatabase db;
    int m_id;
    HcaServer* server;
};

#endif // HCATHREAD_H
