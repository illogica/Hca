#ifndef CREATEROOMWORKER_H
#define CREATEROOMWORKER_H

#include <QObject>
#include <QWebSocket>
#include "hcathread.h"
#include "hcaworker.h"
#include "client.h"

class CreateRoomWorker : public HcaWorker
{
    Q_OBJECT
public:
    CreateRoomWorker(){}
    ~CreateRoomWorker(){}

    int ownerId;
    QString name;
    QString description;
    QString motd;
    QString avatar;
    int worldId;

    QWebSocket* socket;

signals:
    void createRoomResult(QByteArray, QWebSocket*);
    void dbError(QString error);

public slots:
    void doWork(HcaThread* t);

private:
    QString m_id; //Thread id
};

#endif // CREATEROOMWORKER_H
