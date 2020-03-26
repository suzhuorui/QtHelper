#-------------------------------------------------
#
# Project created by QtCreator 2020-02-25T15:09:49
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graduation_project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        common.cpp \
        createtcpclient.cpp \
        createtcpserver2.cpp \
        createudpclient.cpp \
        createudpserver.cpp \
        helpdialog.cpp \
        main.cpp \
        mainwindow.cpp \
        myobject.cpp \
        serialport.cpp \
        settingdialog.cpp \
        socketrun.cpp \
        tcpclient.cpp \
        tcpserver.cpp \
        tcpservermanagement.cpp \
        udpclient.cpp \
        udpserver.cpp \
        settingdialog.cpp

HEADERS += \
        common.h \
        createtcpclient.h \
        createtcpserver2.h \
        createudpclient.h \
        createudpserver.h \
        helpdialog.h \
        mainwindow.h \
        myobject.h \
        serialport.h \
        settingdialog.h \
        socketrun.h \
        tcpclient.h \
        tcpserver.h \
        tcpservermanagement.h \
        udpclient.h \
        udpserver.h \
        settingdialog.h

FORMS += \
        createtcpclient.ui \
        createtcpserver2.ui \
        createudpclient.ui \
        createudpserver.ui \
        helpdialog.ui \
        mainwindow.ui \
        settingdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = mFs.ico


RESOURCES += \
    images.qrc

DISTFILES +=
