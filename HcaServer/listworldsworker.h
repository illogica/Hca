#ifndef LISTWORLDSWORKER_H
#define LISTWORLDSWORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QWebSocket>
#include "hcathread.h"
#include "hcaworker.h"

class ListWorldsWorker : public HcaWorker
{
    Q_OBJECT
public:
    ListWorldsWorker();
    ~ListWorldsWorker(){}

    QWebSocket* socket;

public slots:
    void doWork(HcaThread* t);

signals:
    void listWorldsResult(QByteArray, QWebSocket*);
    void dbError(QString error);

private:
    QSqlDatabase m_db;
    QString m_id;

};

#endif // LISTWORLDSWORKER_H
