#-------------------------------------------------
#
# Project created by QtCreator 2017-10-05T13:57:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cub
TEMPLATE = app
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
LIBS += -lassimp
SOURCES += main.cpp\
        widget.cpp \
    model.cpp \
    mesh.cpp \
    mainwindow.cpp

HEADERS  += widget.h \
    model.h \
    mesh.h \
    mainwindow.h \
    settings.h

FORMS    += \
    mainwindow.ui

DISTFILES += \
    vert.vert \
    frag.frag \
    phongfrag.frag \
    phongvert.vert

RESOURCES += \
    shaders.qrc
