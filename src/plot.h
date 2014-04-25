/****************************************************************************
**
** Copyright (C) 2011 crazyleen <357228736@qq.com>
** All rights reserved.
**
****************************************************************************/
#ifndef __PLOT_H
#define __PLOT_H
#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>
#include "electrocardiogram.h"

class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotDirectPainter;

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot(QWidget * = NULL);
    virtual ~Plot();

    void start();
    virtual void replot();

    void resetViewZome();
    void repaintOnLeft(bool);
    void restartSample(void);
    void saveToFile(void);
    void loadFromFile(QString &loadFile);
public Q_SLOTS:
    void setIntervalLength(double);

protected:
    virtual void showEvent(QShowEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual void timerEvent(QTimerEvent *);

private:

    void initGradient();

    void updateCurve();
    void incrementInterval();

    QwtPlotMarker *d_origin;
    QwtPlotCurve *d_curve;
    int d_paintedPoints;

    QwtPlotDirectPainter *d_directPainter;

    QwtInterval d_interval;
    int d_timerId;
    bool repaintonleft;
    QwtSystemClock d_clock;
};



#endif
