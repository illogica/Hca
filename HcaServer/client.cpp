#include "client.h"

quint32 Client::idCounter = 0;

Client::Client(QObject *parent) : QObject(parent)
{
    id = idCounter++;
}
