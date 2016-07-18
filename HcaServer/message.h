#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>
#include "client.h"

class Message : public QObject
{
    Q_OBJECT
public:
    explicit Message(QObject *parent = 0);
    quint32 id;
    QString body;
    Client from;
    Client to;
    QDateTime dateTime;

    static quint32 idCounter;

signals:

public slots:
};

#endif // MESSAGE_H
