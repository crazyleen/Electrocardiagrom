/****************************************************************************
**
** Copyright (C) 2011 crazyleen <357228736@qq.com>
** All rights reserved.
**
****************************************************************************/
#ifndef _SIGNAL_DATA_H_
#define _SIGNAL_DATA_H_ 1


#include "electrocardiogram.h"
#include <qrect.h>

class SignalData
{
public:
    static SignalData &instance();

    void append(const QPointF &pos);
    void clearStaleValues(double min);

    int size() const;
    QPointF value(int index) const;

    QRectF boundingRect() const;
    void saveToFile(const QString &);
    void loadFromFile(const QString &);
    void lock();
    void unlock();

private:
    SignalData();
    SignalData(const SignalData &);
    SignalData &operator=( const SignalData & );

    virtual ~SignalData();

    class PrivateData;
    PrivateData *d_data;
};

#endif
