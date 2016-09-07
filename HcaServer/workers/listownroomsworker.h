#ifndef LISTOWNROOMSWORKER_H
#define LISTOWNROOMSWORKER_H

#include <QObject>
#include <QWebSocket>
#include "hcathread.h"
#include "hcaworker.h"

class ListOwnRoomsWorker : public HcaWorker
{
public:
    ListOwnRoomsWorker(){}
    ~ListOwnRoomsWorker(){}

    QWebSocket* socket;
    int clientId;

signals:
    void listOwnRoomsWorkerResult(QByteArray, QWebSocket*);
    void dbError(QString error);

public slots:
    void doWork(HcaThread* t);

private:
    QString m_id; //Thread id
};

#endif // LISTOWNROOMSWORKER_H
