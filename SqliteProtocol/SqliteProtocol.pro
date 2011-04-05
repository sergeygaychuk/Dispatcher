#-------------------------------------------------
#
# Project created by QtCreator 2011-03-28T11:19:28
#
#-------------------------------------------------

QT       -= core gui

TARGET = SqliteProtocol
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    sqliteprotocol.c

HEADERS += sqliteprotocol.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../sqlite3
DEPENDPATH += $$PWD/../sqlite3
