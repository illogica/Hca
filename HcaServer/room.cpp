#include "room.h"

quint32 Room::idCounter = 0;

Room::Room(QObject *parent) : QObject(parent)
{
    id = idCounter++;
}
