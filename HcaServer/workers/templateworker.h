#ifndef TEMPLATEWORKER_H
#define TEMPLATEWORKER_H

#include <QObject>
#include <QWebSocket>
#include "hcathread.h"
#include "hcaworker.h"

class TemplateWorker : public HcaWorker
{
    Q_OBJECT
public:
    TemplateWorker(){}
    ~TemplateWorker(){}

    QWebSocket* socket;

signals:
    void templateWorkerResult(QByteArray, QWebSocket*);
    void dbError(QString error);

public slots:
    void doWork(HcaThread* t);

private:
    QString m_id; //Thread id

};

#endif // TEMPLATEWORKER_H
