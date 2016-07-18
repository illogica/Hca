#ifndef ROOM_H
#define ROOM_H

#include <QObject>

class Room : public QObject
{
    Q_OBJECT
    Q_FORWARD_DECLARE_OBJC_CLASS(Client);
    Q_FORWARD_DECLARE_OBJC_CLASS(Conversation);
public:
    explicit Room(QObject *parent = 0);
    quint32 id;
    QString name;
    Client *owner;
    QList<Client *> masters;
    QList<Conversation *> conversations;

    static quint32 idCounter;

signals:

public slots:
};

#endif // ROOM_H
