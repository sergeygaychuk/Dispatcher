#-------------------------------------------------
#
# Project created by QtCreator 2011-04-01T13:37:59
#
#-------------------------------------------------

QT       -= core gui

TARGET = sqlite3
TEMPLATE = lib
CONFIG += staticlib

SOURCES += sqlite3.c

HEADERS += sqlite3.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
