#ifndef HCATHREADPOOL_H
#define HCATHREADPOOL_H

#include <QObject>
#include <hcathread.h>

class HcaThreadPool : public QObject
{
    Q_OBJECT
public:
    explicit HcaThreadPool(QObject *parent = 0);

signals:

public slots:

private:
    QList<HcaThread *> m_threads;
    int m_maxThreads;
};

#endif // HCATHREADPOOL_H
