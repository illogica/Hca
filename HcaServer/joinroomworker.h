#ifndef JOINROOMWORKER_H
#define JOINROOMWORKER_H

#include <QObject>
#include <QWebSocket>
#include "hcathread.h"
#include "hcaworker.h"
#include "client.h"

class JoinRoomWorker : public HcaWorker
{
    Q_OBJECT
public:
    JoinRoomWorker(){}
    ~JoinRoomWorker(){}

    QWebSocket* socket;
    int roomId;
    int clientId;

signals:
    void joinRoomResult(QByteArray, QWebSocket*);
    void dbError(QString error);

public slots:
    void doWork(HcaThread* t);

private:
    QString m_id; //Thread id
};

#endif // JOINROOMWORKER_H
