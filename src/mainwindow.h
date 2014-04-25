/****************************************************************************
**
** Copyright (C) 2011 crazyleen <357228736@qq.com>
** All rights reserved.
**
****************************************************************************/
#include <qwidget.h>
#include <QPushButton>
#include "electrocardiogram.h"

#if LINUX_EMBEDDED_SYSTEM
#else
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include "plot.h"
#endif

//class Plot;
class WheelBox;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget * = NULL);

    void start();
    double signalInterval() const;

public Q_SLOTS:
    void   startSampleOnButtonClick(void);
    void   stopSampleOnButtonClick(void);
    void   saveToFileOnButtonClick(void);
    void   loadFromFileOnButtonClick(void);
    void   resetViewZomeOnButtonClick(void);
    void   xAddOnButtonClick();
    void   xDecOnButtonClick();
    void   aboutMeOnButtonClick();

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

Q_SIGNALS:
    void signalIntervalChanged(double);
    void signalStopSample(void);
    void signalStartSample(void);

private:
    WheelBox *d_timerWheel;
    WheelBox *d_intervalWheel;

    QPushButton *stopSample;
    QPushButton *startSample;
    QPushButton *saveToFile;
    QPushButton *loadFromFile;
    QPushButton *resetViewZome;
    QPushButton *xAdd;
    QPushButton *xDec;
    QPushButton *aboutMe;

    Plot *d_plot;
};
