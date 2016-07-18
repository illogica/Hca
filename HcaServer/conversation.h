#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QObject>
#include <QDateTime>
#include <message.h>

class Conversation : public QObject
{
    Q_OBJECT
    Q_FORWARD_DECLARE_OBJC_CLASS(Client);
public:
    explicit Conversation(QObject *parent = 0);
    quint32 id;
    QDateTime creation;
    QList<Client *> clients;
    QList<Message *> messages;

    static quint32 idCounter;

signals:

public slots:
};

#endif // CONVERSATION_H
