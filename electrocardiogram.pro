QT       += core gui

TARGET = electrocardiogram
TEMPLATE = app

RESOURCES += \
    icons.qrc

HEADERS += \
    src/wheelbox.h \
    src/signaldata.h \
    src/samplingthread.h \
    src/plot.h \
    src/mainwindow.h \
    src/electrocardiogram.h \
    src/curvedata.h

SOURCES += \
    src/wheelbox.cpp \
    src/signaldata.cpp \
    src/samplingthread.cpp \
    src/plot.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/curvedata.cpp

INCLUDEPATH += /usr/local/qwt-6.0.3-svn/include

LIBS += -L/usr/local/qwt-6.0.3-svn/lib -lqwt
