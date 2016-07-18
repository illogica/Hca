#include "message.h"

quint32 Message::idCounter = 0;

Message::Message(QObject *parent) : QObject(parent)
{
    id = idCounter++;
}
