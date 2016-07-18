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
    QString name;
    QList<Room *> rooms;

signals:

public slots:
};

#endif // WORLD_H
