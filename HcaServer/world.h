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

    QString name() const;
    void setName(const QString &name);

    QList<Room *> rooms() const;
    void setRooms(const QList<Room *> &rooms);

signals:

public slots:

private:
    QString m_name;
    QList<Room *> m_rooms;
};

#endif // WORLD_H
