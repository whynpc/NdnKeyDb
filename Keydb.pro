#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T16:08:54
#
#-------------------------------------------------

QT       += core gui

LIBS     += -lsqlite3 -lccn -lcrypto

INCLUDEPATH += /usr/local/include/

TARGET = Keydb
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    keydb.cpp \
    visualizer.cpp \
    dialog.cpp

HEADERS += \
    keydb.h \
    visualizer.h \
    dialog.h
