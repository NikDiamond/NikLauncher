#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T12:26:14
#
#-------------------------------------------------

QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NikLauncher
TEMPLATE = app


SOURCES += main.cpp\
        mainclass.cpp \
    connecter.cpp \
    reg.cpp \
    mdparser.cpp \
    settings.cpp

HEADERS  += mainclass.h \
    connecter.h \
    reg.h \
    mdparser.h \
    settings.h

FORMS    += mainclass.ui \
    reg.ui

OTHER_FILES +=

RESOURCES += \
    images.qrc
