#include "conversation.h"

quint32 Conversation::idCounter = 0;

Conversation::Conversation(QObject *parent) : QObject(parent)
{
    id = idCounter++;
}
