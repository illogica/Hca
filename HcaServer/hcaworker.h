#ifndef HCAWORKER_H
#define HCAWORKER_H

#include <QObject>
#include "hcathread.h"

class HcaWorker : public QObject
{
     Q_OBJECT
public:
    HcaWorker(){}
    ~HcaWorker(){}

public slots:
    void doWork(HcaThread* t){Q_UNUSED(t)}

signals:
};
Q_DECLARE_METATYPE(HcaWorker*)
#endif // HCAWORKER_H
