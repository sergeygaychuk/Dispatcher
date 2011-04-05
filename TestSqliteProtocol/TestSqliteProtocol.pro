#-------------------------------------------------
#
# Project created by QtCreator 2011-03-28T13:02:26
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_testsqliteprotocoltest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_testsqliteprotocoltest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../SqliteProtocol-build-desktop/release/ -lSqliteProtocol
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../SqliteProtocol-build-desktop/debug/ -lSqliteProtocol
else:symbian: LIBS += -lSqliteProtocol
else:unix: LIBS += -L$$PWD/../SqliteProtocol-build-desktop/ -lSqliteProtocol

INCLUDEPATH += $$PWD/../SqliteProtocol
DEPENDPATH += $$PWD/../SqliteProtocol

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../SqliteProtocol-build-desktop/release/SqliteProtocol.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../SqliteProtocol-build-desktop/debug/SqliteProtocol.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../SqliteProtocol-build-desktop/libSqliteProtocol.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../sqlite3-build-desktop/release/ -lsqlite3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../sqlite3-build-desktop/debug/ -lsqlite3
else:symbian: LIBS += -lsqlite3
else:unix: LIBS += -L$$PWD/../sqlite3-build-desktop/ -lsqlite3

INCLUDEPATH += $$PWD/../sqlite3
DEPENDPATH += $$PWD/../sqlite3

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../sqlite3-build-desktop/release/sqlite3.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../sqlite3-build-desktop/debug/sqlite3.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../sqlite3-build-desktop/libsqlite3.a
