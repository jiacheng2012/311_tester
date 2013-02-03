#ifndef CFUNCTIONTESTPRESSUREWINDOW_H
#define CFUNCTIONTESTPRESSUREWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

/**************
 *冷却水温测试
*************/

class CFunctionTestPressureWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFunctionTestPressureWindow(QWidget *parent = 0);

public:
    QSpinBox    *_currentParkingPressureValue,*_currentDrivingPressureValue;
    QSpinBox    *_currentBrakingPressure3Value,*_currentBrakingPressure4Value;
    QPushButton *_autoTestButton;
    QPushButton *_singleTestButton1,*_singleTestButton2,*_singleTestButton3,*_singleTestButton4;
    QLabel      *_pressureLowLabel,*_pressureBuzzerLabel,*_statusLabel;
    int         _cur;
    QTimer      *_timer;
    CMeterConfig           *cf;

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void autoTestButtonClicked();
    void singleTestButton1Clicked();
    void singleTestButton2Clicked();
    void singleTestButton3Clicked();
    void singleTestButton4Clicked();
    void timerUpdate();

};

#endif // CFUNCTIONTESTSPEEDTABLEWINDOW_H
