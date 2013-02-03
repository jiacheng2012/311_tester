#ifndef CFUNCTIONTESTWATERTEMPERATURETABLEWINDOW_H
#define CFUNCTIONTESTWATERTEMPERATURETABLEWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

/**************
 *冷却水温测试
*************/

class CFunctionTestWaterTemperatureWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFunctionTestWaterTemperatureWindow(QWidget *parent = 0);

public:
    QSpinBox    *_currentValue;
    QPushButton *_autoTestButton;
    QPushButton *_singleTestButton;
    QLabel      *_waterTemperatureHighLabel,*_waterBuzzerLabel,*_statusLabel;
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

#endif // CFUNCTIONTESTSPEEDTABLEWINDOW_H
