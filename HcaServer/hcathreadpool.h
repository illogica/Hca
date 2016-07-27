#ifndef HCATHREADPOOL_H
#define HCATHREADPOOL_H

#include <QObject>
#include "hcathread.h"
#include "hcaworker.h"

class HcaThreadPool : public QObject
{
    Q_OBJECT
public:
    explicit HcaThreadPool(QObject *parent = 0);
    void push(HcaWorker* w);

    int maxThreads = 4;

signals:

public slots:
    void setThreadStatus(QString t_id, bool available);
    void threadInitialized(HcaThread* t);

private:
    void applyWorkerToThread(QString t_id, HcaWorker* w);
    void launchNewThread();

    QList<HcaWorker *> m_workersQueue;
    QHash<QString, HcaThread*> m_threads;
    QHash<QString, bool> m_threadsStatus;

};

#endif // HCATHREADPOOL_H
