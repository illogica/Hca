#ifndef PROTOCOL_H
#define PROTOCOL_H

#define REQUEST "request"
#define UUID "uuid"
#define NAME "name"
#define ERROR_BODY "error_body"
#define ROOMS "rooms"
#define ROOM_ID "room_id"
#define ROOM_NAME "room_name"
#define ROOM_SIZE "room_size"
#define WORLDS "worlds"
#define WORLD_ID "world_id"
#define WORLD_NAME "world_name"
#define WORLD_SIZE "world_size"
#define DESCRIPTION "description"

enum Privilege{
    GUEST,
    USER,
    MASTER,
    ADMIN
};

enum Status{
    OFFLINE,
    ONLINE,
    INVISIBLE,
};

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
