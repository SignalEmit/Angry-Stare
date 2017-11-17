#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T08:57:12
#
#-------------------------------------------------
QT += sql
QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = denglujiemian
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        widget.cpp \
    interface.cpp \
    pageme.cpp \
    dialog.cpp \
    productwindows.cpp \
    chat.cpp \
    dialoglogin.cpp \
    tcpclass.cpp \
    search.cpp \
    picturethread.cpp \
    mysettingpage.cpp \
    choosefile.cpp \
    MyScrollArea.cpp \
    orderforgoods.cpp

HEADERS  += widget.h \
    interface.h \
    pageme.h \
    dialog.h \
    productwindows.h \
    chat.h \
    dialoglogin.h \
    tcpclass.h \
    search.h \
    picturethread.h \
    mysettingpage.h \
    choosefile.h \
    MyScrollArea.h \
    orderforgoods.h

FORMS    += widget.ui \
    interface.ui \
    pageme.ui \
    dialog.ui \
    productwindows.ui \
    chat.ui \
    dialoglogin.ui \
    search.ui \
    mysettingpage.ui \
    choosefile.ui \
    orderforgoods.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    nidaye.qrc

DISTFILES += \
    AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD
