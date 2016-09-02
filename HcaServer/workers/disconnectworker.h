#ifndef DISCONNECTWORKER_H
#define DISCONNECTWORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QWebSocket>
#include "hcathread.h"
#include "hcaworker.h"

class DisconnectWorker : public HcaWorker
{
    Q_OBJECT
public:
    DisconnectWorker(){}
    ~DisconnectWorker(){}

    int id;

public slots:
    void doWork(HcaThread* t);

signals:
    void disconnectResult();
    void dbError(QString error);

private:
    QString m_id;
};

#endif // DISCONNECTWORKER_H
