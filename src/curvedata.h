/****************************************************************************
**
** Copyright (C) 2011 crazyleen <357228736@qq.com>
** All rights reserved.
**
****************************************************************************/
#include <qwt_series_data.h>
#include <qpointer.h>
#include "electrocardiogram.h"


class SignalData;

class CurveData: public QwtSeriesData<QPointF>
{
public:
    const SignalData &values() const;
    SignalData &values();

    virtual QPointF sample(size_t i) const;
    virtual size_t size() const;

    virtual QRectF boundingRect() const;
};
