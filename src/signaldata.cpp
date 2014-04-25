/****************************************************************************
**
** Copyright (C) 2011 crazyleen <357228736@qq.com>
** All rights reserved.
**
****************************************************************************/
#include "signaldata.h"
#include <qvector.h>
#include <qmutex.h>
#include <qreadwritelock.h>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

class SignalData::PrivateData
{
public:
    PrivateData():
        boundingRect(1.0, 1.0, -2.0, -2.0) // invalid
    {
        //values.reserve(1000);
    }

    inline void append(const QPointF &sample)
    {
        values.append(sample);

        // adjust the bounding rectangle

        if ( boundingRect.width() < 0 || boundingRect.height() < 0 )
        {
            boundingRect.setRect(sample.x(), sample.y(), 0.0, 0.0);
        }
        else
        {
            boundingRect.setRight(sample.x());

            if ( sample.y() > boundingRect.bottom() )
                boundingRect.setBottom(sample.y());

            if ( sample.y() < boundingRect.top() )
                boundingRect.setTop(sample.y());
        }
    }

    QReadWriteLock lock;

    QVector<QPointF> values;
    QRectF boundingRect;

    QMutex mutex; // protecting pendingValues
    QVector<QPointF> pendingValues;
};

SignalData::SignalData()
{
    d_data = new PrivateData();
}

SignalData::~SignalData()
{
    delete d_data;
}

int SignalData::size() const
{
    return d_data->values.size();
}   

QPointF SignalData::value(int index) const
{
    return d_data->values[index];
}   

QRectF SignalData::boundingRect() const
{
    return d_data->boundingRect;
}

void SignalData::lock()
{
    d_data->lock.lockForRead();
}

void SignalData::unlock()
{
    d_data->lock.unlock();
}

void SignalData::append(const QPointF &sample)
{
    d_data->mutex.lock();
    d_data->pendingValues += sample;

    const bool isLocked = d_data->lock.tryLockForWrite();
    if ( isLocked )
    {
        const int numValues = d_data->pendingValues.size();
        const QPointF *pendingValues = d_data->pendingValues.data();

        for ( int i = 0; i < numValues; i++ )
            d_data->append(pendingValues[i]);

        d_data->pendingValues.clear();

        d_data->lock.unlock();
    }

    d_data->mutex.unlock();
}


void SignalData::saveToFile(const QString &fileName)
{
    d_data->lock.lockForWrite();

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {
            return;
        }

        QDataStream out(&file);
        //out.setVersion(QDataStream::Qt_4_3);
        out << d_data->values;
    }

    d_data->lock.unlock();
}

void SignalData::loadFromFile(const QString &fileName)
{
    if (fileName.isEmpty())
        return;

    d_data->lock.lockForWrite();
    //d_data->values.clear();
    {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            d_data->lock.unlock();
            return;
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_3);
        d_data->values.empty();   // empty existing values
        in >> d_data->values;
    }

    d_data->lock.unlock();
}

void SignalData::clearStaleValues(double limit)
{
    d_data->lock.lockForWrite();

    d_data->boundingRect = QRectF(1.0, 1.0, -2.0, -2.0); // invalid

    const QVector<QPointF> values = d_data->values;
    d_data->values.clear();
    d_data->values.reserve(1024 * 1024);

#if 0
    int index;
    for ( index = values.size() - 1; index >= 0; index-- )
    {
        if ( values[index].x() < limit )
            break;
    }

    if ( index > 0 )
        d_data->append(values[index++]);

    while ( index < values.size() - 1 )
        d_data->append(values[index++]);
#endif
    d_data->lock.unlock();
}

SignalData &SignalData::instance()
{
    static SignalData valueVector;
    return valueVector;
}
