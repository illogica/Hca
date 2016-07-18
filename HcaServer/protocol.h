#ifndef PROTOCOL_H
#define PROTOCOL_H

enum Request{
    ERROR =0,
    PING,
    PONG,
    LOGIN,
    LIST_ROOMS,
    LIST_WORLDS,
    JOIN_ROOM,
    JOIN_WORLD,
    SEND_ROOM,
    SEND_USER
};

#endif // PROTOCOL_H
