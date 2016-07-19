#ifndef ROOM_H
#define ROOM_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include "protocol.h"
#include "client.h"

class Room : public QObject
{
    Q_OBJECT
    Q_FORWARD_DECLARE_OBJC_CLASS(Conversation);
public:
    explicit Room(QObject *parent = 0);
    quint32 id;
    static quint32 idCounter;
    bool isEmpty(){return m_clients.isEmpty();}

    void addClient(Client* client);
    void removeClient(Client* client);

    QString name() const;
    void setName(const QString &name);

    Client *owner() const;
    void setOwner(Client *owner);

signals:

public slots:
    void notifyJoin(const QString &name);
    void notifyLeave(const QString &name);
    void notifyChangeName(const QString &name);


private:
    QString m_name;
    Client *m_owner;
    QList<Client *> m_masters;  //room operators
    QList<Client *> m_clients;  //room users (and operators, too)
    QList<Conversation *> m_conversations;
};

#endif // ROOM_H
