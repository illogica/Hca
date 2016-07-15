QT += core websockets
QT -= gui

CONFIG += c++11

TARGET = HcaServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    hcaserver.cpp \
    client.cpp

HEADERS += \
    hcaserver.h \
    client.h \
    protocol.h

DISTFILES += \
    Protocol.txt
