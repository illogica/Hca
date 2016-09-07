QT += core websockets sql testlib
QT -= gui

CONFIG += c++11

TARGET = HcaServer
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += workers

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
    dbmanager.cpp \
    workers/disconnectworker.cpp \
    workers/listworldsworker.cpp \
    workers/listroomsworker.cpp \
    workers/joinroomworker.cpp \
    workers/createroomworker.cpp \
    workers/leaveroomworker.cpp \
    workers/loginworker.cpp \
    workers/templateworker.cpp \
    test/testserver.cpp \
    workers/listownroomsworker.cpp

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
    hcaworker.h \
    dbmanager.h \
    workers/disconnectworker.h \
    workers/listworldsworker.h \
    workers/listroomsworker.h \
    workers/joinroomworker.h \
    workers/createroomworker.h \
    workers/leaveroomworker.h \
    workers/loginworker.h \
    workers/templateworker.h \
    test/testserver.h \
    workers/listownroomsworker.h

DISTFILES += \
    Protocol.txt \
    sql/init.sql
