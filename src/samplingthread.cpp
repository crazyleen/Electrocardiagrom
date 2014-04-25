/****************************************************************************
**
** Copyright (C) 2011 crazyleen <357228736@qq.com>
** All rights reserved.
**
****************************************************************************/
#include "samplingthread.h"
#include "signaldata.h"
#include <qwt_math.h>
#include <math.h>

#if LINUX_EMBEDDED_SYSTEM
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <errno.h>
#include <string.h>
#endif

#if QT_VERSION < 0x040600
#define qFastSin(x) ::sin(x)
#endif

SamplingThread::SamplingThread(QObject *parent):
    QwtSamplingThread(parent)
{
#if LINUX_EMBEDDED_SYSTEM
    fd = open("/dev/adc", 0);
     if (fd < 0) {
           perror("open ADC device:");
    }
#endif
}

SamplingThread::~SamplingThread()
{
#if LINUX_EMBEDDED_SYSTEM
    close(fd);
#endif
}

void SamplingThread::sample(double elapsed)
{
    const QPointF s(elapsed, value(elapsed));
    SignalData::instance().append(s);
}

double SamplingThread::value(double timeStamp) const
{
#if LINUX_EMBEDDED_SYSTEM
    int value = -1;
    char buffer[5];

    int len = read(fd, buffer, sizeof buffer -1);
    if (len > 0) {
        buffer[len] = '\0';
        sscanf(buffer, "%d", &value);
    } else {
         perror("read ADC device:");
    }
    const double v = (double)value;
    return v;
#else
    const double x = timeStamp;
    const double v = 100 * qFastSin(x * 10) + 500;
    return v;
#endif
}
