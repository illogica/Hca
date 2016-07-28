#ifndef LISTROOMSWORKER_H
#define LISTROOMSWORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QWebSocket>
#include "hcathread.h"
#include "hcaworker.h"

class ListRoomsWorker : public HcaWorker
{
    Q_OBJECT
public:
    ListRoomsWorker();
    ~ListRoomsWorker(){}

    QWebSocket* socket;
    qint32 worldId;

public slots:
    void doWork(HcaThread* t);

signals:
    void listRoomsResult(QByteArray, QWebSocket*);
    void dbError(QString error);

private:
    QSqlDatabase m_db;
    QString m_id;
};

#endif // LISTROOMSWORKER_H
