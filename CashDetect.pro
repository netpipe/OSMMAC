#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T18:15:50
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = CashDetect
TEMPLATE = app
DESTDIR = $$PWD

INCLUDEPATH += /usr/include/opencv
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lopencv_calib3d
LIBS += -lopencv_core -lopencv_dnn -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml
LIBS += -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video
LIBS += -lopencv_videoio -lopencv_videostab -lopencv_ffmpeg

SOURCES += main.cpp \
    gui/mainwindow.cpp \
    camera.cpp \
    gui/displaywidget.cpp \
    gui/imageviewer.cpp \
    DetectThread.cpp

HEADERS += gui/mainwindow.h \
    camera.h \
    gui/displaywidget.h \
    gui/imageviewer.h \
    DetectThread.h
