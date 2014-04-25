/****************************************************************************
**
** Copyright (C) 2011 crazyleen <357228736@qq.com>
** All rights reserved.
**
****************************************************************************/
#include <qapplication.h>
#include "mainwindow.h"
#include "samplingthread.h"
#include "electrocardiogram.h"



int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));


    MainWindow window;

#if LINUX_EMBEDDED_SYSTEM
    window.setWindowFlags(window.windowFlags() &~ Qt::WindowCloseButtonHint);
    window.setWindowFlags(window.windowFlags()& ~Qt::WindowMaximizeButtonHint&  ~Qt::WindowMinimizeButtonHint);
    window.setWindowFlags(Qt::FramelessWindowHint);
    window.showMaximized();
#else
    window.resize(800,400);

#endif





    SamplingThread samplingThread;
    samplingThread.setInterval(window.signalInterval());

    window.connect(&window, SIGNAL(signalIntervalChanged(double)),
        &samplingThread, SLOT(setInterval(double)));
    window.connect(&window, SIGNAL(signalStartSample(void)),
        &samplingThread, SLOT(start(void)));
    window.connect(&window, SIGNAL(signalStopSample(void)),
        &samplingThread, SLOT(stop(void)));

    window.show();

    samplingThread.start();
    window.start();

    bool ok = app.exec(); 

    samplingThread.stop();
    samplingThread.wait(1000);

    return ok;
}
