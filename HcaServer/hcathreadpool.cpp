#include "hcathreadpool.h"

HcaThreadPool::HcaThreadPool(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<HcaWorker*>();
    qRegisterMetaType<HcaThread*>();
    for(int i=0; i<maxThreads; i++){
        qWarning() << "ThreadPool creating thread " << i;
        launchNewThread();
    }
}

void HcaThreadPool::push(HcaWorker *w)
{
    //search for an available thread
    for(QString thread_id: m_threadsStatus.keys()){
        if(m_threadsStatus.value(thread_id)){
            applyWorkerToThread(thread_id, w);
            return;
        }
    }
    //no available thread found, enqueue worker and launch new thread
    launchNewThread();
    m_workersQueue.append(w);
}

void HcaThreadPool::setThreadStatus(QString t_id, bool available)
{
    m_threadsStatus.insert(t_id, available);
    if(available)
        qWarning() << "Thread " << t_id << " becomes available";
    else
        qWarning() << "Thread " << t_id << " becomes busy";

    //once a thread becomes available, look for enqueued jobs
    if(available){
        if(!m_workersQueue.isEmpty()){
            qWarning() << "Thread " << t_id << " picks up job from queue";
            applyWorkerToThread(t_id, m_workersQueue.takeFirst());
        }
    }
}

void HcaThreadPool::threadInitialized(HcaThread *t)
{
    qWarning() << "Thread " << t->id() << " is ready";
    m_threads.insert(t->id(), t);
    setThreadStatus(t->id(), true);
}

void HcaThreadPool::applyWorkerToThread(QString t_id, HcaWorker *w)
{
    HcaThread* t = m_threads.value(t_id);
    w->moveToThread(t);
    QMetaObject::invokeMethod(w, "doWork", Qt::QueuedConnection, Q_ARG(HcaThread*, t));
}

void HcaThreadPool::launchNewThread()
{
    HcaThread* t = new HcaThread();
    connect(t, &HcaThread::initialized, this, &HcaThreadPool::threadInitialized);
    connect(t, &HcaThread::setThreadStatus, this, &HcaThreadPool::setThreadStatus);
    t->start();
}
