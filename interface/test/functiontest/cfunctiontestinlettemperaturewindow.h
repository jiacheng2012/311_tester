#ifndef CFUNCTIONTESTINNETTEMPERATUREWINDOW_H
#define CFUNCTIONTESTINNETTEMPERATUREWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

/**************
 *进气温度测试
*************/

class CFunctionTestInletTemperatureWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFunctionTestInletTemperatureWindow(QWidget *parent = 0);

public:
    QSpinBox    *_currentValue;
    QPushButton *_autoTestButton;
    QPushButton *_singleTestButton;
    QLabel      *_fanHighSpeedLabel,*_fanLowSpeedLabel,*_statusLabel;

    int         _cur;
    QTimer      *_timer;
    CMeterConfig           *cf;

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void autoTestButtonClicked();
    void singleTestButtonClicked();
    void timerUpdate();

};

#endif // CFUNCTIONTESTINNETTEMPERATUREWINDOW_H
