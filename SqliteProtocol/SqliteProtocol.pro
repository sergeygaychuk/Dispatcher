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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../sqlite3-build-desktop/release/ -lsqlite3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../sqlite3-build-desktop/debug/ -lsqlite3
else:symbian: LIBS += -lsqlite3
else:unix: LIBS += -L$$PWD/../sqlite3-build-desktop/ -lsqlite3

INCLUDEPATH += $$PWD/../sqlite3
DEPENDPATH += $$PWD/../sqlite3

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../sqlite3-build-desktop/release/sqlite3.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../sqlite3-build-desktop/debug/sqlite3.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../sqlite3-build-desktop/libsqlite3.a
