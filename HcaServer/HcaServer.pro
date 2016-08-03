QT += core websockets sql
QT -= gui

CONFIG += c++11

TARGET = HcaServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    hcaserver.cpp \
    client.cpp \
    message.cpp \
    conversation.cpp \
    room.cpp \
    world.cpp \
    hcathreadpool.cpp \
    hcathread.cpp \
    loginworker.cpp \
    disconnectworker.cpp \
    listworldsworker.cpp \
    listroomsworker.cpp \
    dbmanager.cpp

HEADERS += \
    hcaserver.h \
    client.h \
    protocol.h \
    message.h \
    conversation.h \
    room.h \
    world.h \
    hcathreadpool.h \
    hcathread.h \
    pingrunnable.h \
    loginworker.h \
    hcaworker.h \
    disconnectworker.h \
    listworldsworker.h \
    listroomsworker.h \
    dbmanager.h

DISTFILES += \
    Protocol.txt \
    sql/init.sql
