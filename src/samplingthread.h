/****************************************************************************
**
** Copyright (C) 2011 crazyleen <357228736@qq.com>
** All rights reserved.
**
****************************************************************************/
#include <qwt_sampling_thread.h>
#include "electrocardiogram.h"

class SamplingThread: public QwtSamplingThread
{
    Q_OBJECT

public:
    SamplingThread(QObject *parent = NULL);
    ~SamplingThread();

protected:
    virtual void sample(double elapsed);

private:
#if LINUX_EMBEDDED_SYSTEM
    int fd;
#endif
    virtual double value(double timeStamp) const;
};
