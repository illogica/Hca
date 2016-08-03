#ifndef WORLD_H
#define WORLD_H

#include <QObject>
#include <QJsonObject>
#include "room.h"

class World : public QObject
{
    Q_OBJECT
public:
    explicit World(QObject *parent = 0);
    ~World(){
        while(!m_rooms.isEmpty()) delete m_rooms.takeFirst();
    }

    QJsonObject toJsonObject();

    int size() {return m_rooms.size();}

    Room *findRoom(const QString &name);
    Room *createRoom();
    void deleteRoom(Room *r);

    QString name() const;
    void setName(const QString &name);

    QList<Room *> rooms() const;
    void setRooms(const QList<Room *> &rooms);

    QString description() const;
    void setDescription(const QString &description);

    int id() const;
    void setId(int id);

    int count() const;
    void setCount(int count);

signals:

public slots:

private:
    int m_id;
    QString m_name;
    QString m_description;
    int m_count;
    QList<Room *> m_rooms;
};

#endif // WORLD_H
