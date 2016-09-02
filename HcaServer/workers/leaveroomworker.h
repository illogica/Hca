#ifndef LEAVEROOMWORKER_H
#define LEAVEROOMWORKER_H

#include <QObject>
#include <QWebSocket>
#include "hcathread.h"
#include "hcaworker.h"

class LeaveRoomWorker : public HcaWorker
{
    Q_OBJECT
public:
    LeaveRoomWorker(){}
    ~LeaveRoomWorker(){}

    int clientId;
    int roomdId;
    QWebSocket* socket;

signals:
    void leaveRoomResult(QByteArray, QWebSocket*);
    void dbError(QString error);

public slots:
    void doWork(HcaThread* t);

private:
    QString m_id; //Thread i
};

#endif // LEAVEROOMWORKER_H
