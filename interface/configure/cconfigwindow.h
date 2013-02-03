#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QWidget>
#include <QtGui>
#include "cconfigadjustmeterappwindow.h"
#include "cconfigadjustparmswindow.h"
#include "cconfigchannelwindow.h"

class CConfigWindow : public QWidget
{
    Q_OBJECT

public:
    CConfigWindow(QWidget *parent = 0);
    ~CConfigWindow();

    QTabWidget *group;

private:
    CConfigAdjustMeterAppWindow* _mMeterAppWnd;
    CConfigChannelWindow* _mChannelWnd; //通道参数配置
    CConfigAdjustParmsWindow* _mAdjustParmsWnd; //调校参数设置 窗体部件
//protected:
//    virtual void paintEvent(QPaintEvent *);
public slots:

private slots:

};

#endif // CONFIGWINDOW_H
