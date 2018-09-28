#-------------------------------------------------
#
# Project created by QtCreator 2017-11-16T16:26:06
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix: DEFINES += USE_GL

contains(DEFINES, USE_GL) {
    QT += opengl
}

TARGET = cdgplay
TEMPLATE = app

target.path = /usr/bin/

INSTALLS += target

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        cdgvideosurface.cpp \
        cdgvideowidget.cpp \
        settings.cpp \
        dlgcdg.cpp \
        libCDG/src/libCDG_Frame_Image.cpp \
        libCDG/src/libCDG_Color.cpp \
        libCDG/src/libCDG.cpp \
    arguments.cpp


HEADERS += \
        cdgvideosurface.h \
        cdgvideowidget.h \
        settings.h \
        dlgcdg.h \
        libCDG/include/libCDG.h \
        libCDG/include/libCDG_Frame_Image.h \
        libCDG/include/libCDG_Color.h \
    arguments.h

FORMS += \
        dlgcdg.ui
