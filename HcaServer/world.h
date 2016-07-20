#ifndef WORLD_H
#define WORLD_H

#include <QObject>
#include "room.h"

class World : public QObject
{
    Q_OBJECT
public:
    explicit World(QObject *parent = 0);
    quint32 id;
    static quint32 idCounter;

    int size() {return m_rooms.size();}

    Room *findRoom(const QString &name);
    Room *createRoom();

    QString name() const;
    void setName(const QString &name);

    QList<Room *> rooms() const;
    void setRooms(const QList<Room *> &rooms);

    QString description() const;
    void setDescription(const QString &description);

signals:

public slots:

private:
    QString m_name;
    QString m_description;
    QList<Room *> m_rooms;
};

#endif // WORLD_H
