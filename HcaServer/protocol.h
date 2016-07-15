#ifndef PROTOCOL_H
#define PROTOCOL_H

enum Request{
    ERROR =0,
    PING =1,
    LOGIN =2,
    LIST_ROOMS =3,
    LIST_WORLDS =4,
    JOIN_ROOM =5,
    JOIN_WORLD =6,
    SEND_ROOM =7,
    SEND_USER =8
};

#endif // PROTOCOL_H
