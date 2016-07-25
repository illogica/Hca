#include "hcathreadpool.h"

HcaThreadPool::HcaThreadPool(QObject *parent) : QObject(parent)
{
    m_maxThreads = 4;
    for(int i=0; i<m_maxThreads; i++){
        qWarning() << "ThreadPool creating thread " << i;
        m_threads.append(new HcaThread(QString(i)));
    }
}
