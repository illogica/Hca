#ifndef PINGRUNNABLE_H
#define PINGRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include "protocol.h"

class PingRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    QWebSocket *socket;

    void run(){
        QJsonObject response;
        response[REQUEST] = PONG;
        QJsonDocument doc;
        doc.setObject(response);
        qWarning() << "Emitting ping from " << QThread::currentThreadId();
        emit pingResult(doc.toJson(QJsonDocument::Compact), socket);
    }


signals:
    void pingResult(QByteArray, QWebSocket*);
};

#endif // PINGRUNNABLE_H
