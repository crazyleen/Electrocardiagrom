/****************************************************************************
**
** Copyright (C) 2011 crazyleen <357228736@qq.com>
** All rights reserved.
**
****************************************************************************/
#include "mainwindow.h"
#include "plot.h"
#include "electrocardiogram.h"
#include "wheelbox.h"
#include <qwt_scale_engine.h>
#include <qlabel.h>
#include <qlayout.h>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
 #include <QStyle>

class CurveData;
MainWindow::MainWindow(QWidget *parent):
    QWidget(parent)
{
    //QSize iconSize(36, 36);

    const double intervalLength = 2.5; // seconds

    d_plot = new Plot(this);
    d_plot->setIntervalLength(intervalLength);

    d_intervalWheel = new WheelBox("Displayed [s]", 0.5, 60.0, 0.5, this);
    d_intervalWheel->setValue(intervalLength);

    d_timerWheel = new WheelBox("Sample Interval [ms]", 1.0, 20.0, 0.1, this);
    d_timerWheel->setValue(10.0);

#if LINUX_EMBEDDED_SYSTEM
    d_intervalWheel->hide();
    d_timerWheel->hide();
#else
    setAcceptDrops(true);
    setWindowIcon(QIcon(":images/electrocardiogram.ico"));
#endif

    startSample = new QPushButton(tr("start"));
    startSample->setToolTip(tr("start sampling data..."));
    startSample->setEnabled(false);
    startSample->setIcon(style()->standardIcon(QStyle::QStyle::QStyle::QStyle::SP_MediaPlay));

    stopSample = new QPushButton(tr("stop"));
    stopSample->setToolTip(tr("Stop sampling data..."));
    stopSample->setEnabled(true);
    stopSample->setIcon(style()->standardIcon(QStyle::QStyle::QStyle::SP_MediaStop));

    saveToFile = new QPushButton(tr("save"));
    saveToFile->setToolTip(tr("save data to file..."));
    saveToFile->setEnabled(false);
    saveToFile->setIcon(style()->standardIcon(QStyle::QStyle::SP_DialogSaveButton));

    loadFromFile = new QPushButton(tr("load"));
    //loadFromFile->adjustSize();
    loadFromFile->setToolTip(tr("load data from file..."));
    loadFromFile->setEnabled(false);
    loadFromFile->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    //loadFromFile->setIconSize(iconSize);


    resetViewZome = new QPushButton(tr("viewZome"));
    resetViewZome->setToolTip(tr("reset view zome"));
    resetViewZome->setEnabled(true);

    xAdd = new QPushButton(tr("zome +"));
    xAdd->setToolTip(tr("X +"));

    xDec = new QPushButton(tr("zome -"));
    xDec->setToolTip(tr("X -"));

    aboutMe = new QPushButton(tr("about"));


    QLabel *appIcon = new QLabel;
    appIcon->setPixmap(QPixmap(":images/electrocardiogram.ico"));

    QVBoxLayout* vLayout1 = new QVBoxLayout();
    vLayout1->addWidget(startSample);
    vLayout1->addWidget(stopSample);
    vLayout1->addWidget(saveToFile);
    vLayout1->addWidget(loadFromFile);
    vLayout1->addWidget(resetViewZome);
    vLayout1->addWidget(xAdd);
    vLayout1->addWidget(xDec);
    vLayout1->addWidget(d_intervalWheel);
    vLayout1->addWidget(d_timerWheel);
    vLayout1->addWidget(aboutMe);
    vLayout1->addStretch(5);
    vLayout1->addWidget(appIcon);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(d_plot, 10);
    layout->addLayout(vLayout1);

    connect(startSample, SIGNAL(clicked()),
        this, SLOT(startSampleOnButtonClick()));
    connect(stopSample, SIGNAL(clicked()),
        this, SLOT(stopSampleOnButtonClick()));
    connect(saveToFile, SIGNAL(clicked()),
        this, SLOT(saveToFileOnButtonClick()));
    connect(loadFromFile, SIGNAL(clicked()),
        this, SLOT(loadFromFileOnButtonClick()));
    connect(resetViewZome, SIGNAL(clicked()),
        this, SLOT(resetViewZomeOnButtonClick()));
    connect(xAdd, SIGNAL(clicked()),
        this, SLOT(xAddOnButtonClick()));
    connect(xDec, SIGNAL(clicked()),
        this, SLOT(xDecOnButtonClick()));
    connect(aboutMe, SIGNAL(clicked()),
        this, SLOT(aboutMeOnButtonClick()));

    connect(d_timerWheel, SIGNAL(valueChanged(double)),
        SIGNAL(signalIntervalChanged(double)));
    connect(d_intervalWheel, SIGNAL(valueChanged(double)),
        d_plot, SLOT(setIntervalLength(double)) );

}

void MainWindow::startSampleOnButtonClick(void)
{
    //QMessageBox::question(this, tr("start sample"), tr("Start sampling data now..."));

    QMessageBox   mb(tr("start sample"), tr("do you want to drop old data and start sampling data?"),
       QMessageBox::Question,
       QMessageBox::Yes | QMessageBox::Default,
       QMessageBox::No  | QMessageBox::Escape,
       QMessageBox::NoButton);
    if(mb.exec() == QMessageBox::No)
       return;

    Q_EMIT signalStartSample();

    startSample->setEnabled(false);
    stopSample->setEnabled(true);
    saveToFile->setEnabled(false);
    loadFromFile->setEnabled(false);
    d_plot->repaintOnLeft(true);
    d_plot->restartSample();
    d_plot->replot();
}

void MainWindow::stopSampleOnButtonClick(void)
{
    //QMessageBox::information(this, tr("button"), tr("button click"));
    QMessageBox   mb(tr("stop sample"), tr("do you want to stop sampling data?"),
       QMessageBox::Question,
       QMessageBox::Yes | QMessageBox::Default,
       QMessageBox::No  | QMessageBox::Escape,
       QMessageBox::NoButton);
    if(mb.exec() == QMessageBox::No)
       return;

    Q_EMIT signalStopSample();
    startSample->setEnabled(true);
    stopSample->setEnabled(false);
    saveToFile->setEnabled(true);
    loadFromFile->setEnabled(true);
    d_plot->repaintOnLeft(FALSE);
}

void MainWindow::saveToFileOnButtonClick(void)
{
    //QMessageBox::information(this, tr("save"), tr("save to file..."));
    d_plot->saveToFile();

}
void MainWindow::loadFromFileOnButtonClick(void)
{
    //QMessageBox::information(this, tr("load"), tr("load file..."));
    QString t = NULL;
    d_plot->loadFromFile(t);
}

void MainWindow::resetViewZomeOnButtonClick()
{
    //QMessageBox::information(this, tr("reset view zome"), tr("reset view zome"));
    d_intervalWheel->setValue(5.0);
    d_plot->resetViewZome();
}

void MainWindow::aboutMeOnButtonClick()
{
    QMessageBox::about(this, tr("About"),
                       tr("<h1>Electrocardiogram</h1>"
                          "<p>southern medical university</p>"
                          "<p>school of biomedical engineering</p>"
                          "by ruisheng <ruishengleen@gmail.com>"));
}

void MainWindow::xAddOnButtonClick()
{
    double value = d_intervalWheel->value();

    if(value > 10 && value < 60)
        value = value + value / 8;
    else if(value > 5)
        value += 1;
    else
        value += 0.5;

    d_intervalWheel->setValue(value);
}

void MainWindow::xDecOnButtonClick()
{
    double value = d_intervalWheel->value();

    if(value > 10)
        value = value - value / 8;
    else if(value > 5)
        value -= 1;
    else if(value >= 1)
        value = 0.5;

    d_intervalWheel->setValue(value);
}

void MainWindow::start()
{
    d_plot->start();
}

double MainWindow::signalInterval() const
{
    return d_timerWheel->value();
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    QString fileName = urls.first().toLocalFile();

    if (! fileName.contains(".qvpf", Qt::CaseInsensitive))
        return;

    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    QString fileName = urls.first().toLocalFile();

    if (! fileName.contains(".qvpf", Qt::CaseInsensitive))
        return;

    Q_EMIT signalStopSample();
    startSample->setEnabled(true);
    stopSample->setEnabled(false);
    saveToFile->setEnabled(true);
    loadFromFile->setEnabled(true);
    d_plot->repaintOnLeft(FALSE);

    d_plot->loadFromFile(fileName);


}
