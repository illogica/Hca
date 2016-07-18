QT += core websockets
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
    world.cpp

HEADERS += \
    hcaserver.h \
    client.h \
    protocol.h \
    message.h \
    conversation.h \
    room.h \
    world.h

DISTFILES += \
    Protocol.txt
