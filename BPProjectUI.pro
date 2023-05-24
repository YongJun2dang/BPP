#-------------------------------------------------
#
# Project created by QtCreator 2017-09-14T13:58:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BPProjectUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    configdialog.cpp \
    handler.cpp \
    camerathread.cpp \
    lmotor.cpp \
    lmockmotor.cpp \
    lyongmotor.cpp \
    motor.cpp \
    motorunit.cpp \
    io.cpp \
    iounit.cpp \
    cameragrabthread.cpp \
    trackinglabel.cpp

HEADERS  += mainwindow.h \
    configdialog.h \
    handler.h \
    camerathread.h \
    lmotor.h \
    lmockmotor.h \
    lyongmotor.h \
    motor.h \
    motorunit.h \
    io.h \
    iounit.h \
    main.h \
    cameragrabthread.h \
    trackinglabel.h

FORMS    += mainwindow.ui \
    configdialog.ui \
    motor.ui \
    motorunit.ui \
    io.ui \
    iounit.ui

#opencv
INCLUDEPATH = -l/usr/local/include

LIBS += -L/usr/local/cuda-6.5/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
