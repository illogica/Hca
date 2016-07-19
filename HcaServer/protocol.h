#ifndef PROTOCOL_H
#define PROTOCOL_H

#define REQUEST "request"
#define UUID "uuid"
#define NAME "name"
#define ERROR_BODY "error_body"
#define ROOM_NAME "room_name"

enum Request{
    ERROR =0,
    PING,
    PONG,
    LOGIN,
    JOIN_ROOM, //join or create is the same
    NOTIFY_JOIN_ROOM,
    NOTIFY_CHANGE_ROOM_NAME,
    LEAVE_ROOM,
    NOTIFY_LEAVE_ROOM,
    LIST_ROOMS,
    LIST_WORLDS,
    JOIN_WORLD,
    SEND_ROOM,
    SEND_USER
};

#endif // PROTOCOL_H
