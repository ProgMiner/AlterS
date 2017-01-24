#-------------------------------------------------
#
# Project created by QtCreator 2017-01-15T22:51:08
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlterS
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
        mainwindow.cpp \
    mainpagewidget.cpp \
    accountpagewidget.cpp \
    settingspagewidget.cpp

HEADERS  += mainwindow.h \
        main.h \
        account.h \
    mainpagewidget.h \
    accountpagewidget.h \
    settingspagewidget.h

FORMS    += mainwindow.ui \
    mainpagewidget.ui \
    accountpagewidget.ui \
    settingspagewidget.ui

unix|win32: LIBS += -LC:/OpenSSL-Win32/lib/ -llibeay32

INCLUDEPATH += C:/OpenSSL-Win32/include
DEPENDPATH += C:/OpenSSL-Win32/include

RESOURCES += \
    resources.qrc
