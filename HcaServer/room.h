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

    static quint32 idCounter;

    QString name() const;
    void setName(const QString &name);

    Client *owner() const;
    void setOwner(Client *owner);

signals:

public slots:

private:
    quint32 id;
    QString m_name;
    Client *m_owner;
    QList<Client *> m_masters;
    QList<Conversation *> m_conversations;
};

#endif // ROOM_H
