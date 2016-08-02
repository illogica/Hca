TEMPLATE = app

QT += qml quick websockets
CONFIG += c++11

SOURCES += main.cpp \
    hcaclient.cpp \
    roomdata.cpp \
    worlddata.cpp

RESOURCES += qml.qrc \
    emojione.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    hcaclient.h \
    roomdata.h \
    worlddata.h

DISTFILES += \
    components/qmldir
