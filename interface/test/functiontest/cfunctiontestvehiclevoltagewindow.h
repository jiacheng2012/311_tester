#ifndef CFUNCTIONTESTVEHICLEVOLTAGEWINDOW_H
#define CFUNCTIONTESTVEHICLEVOLTAGEWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

/**************
 *整车电压测试
*************/

class CFunctionTestVeHicleVoltageWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFunctionTestVeHicleVoltageWindow(QWidget *parent = 0);

public:
    QSpinBox    *_currentValue2;
    QComboBox   *_currentValue1;
    QPushButton *_autoTestButton;
    QPushButton *_singleTestButton1,*_singleTestButton2;
    QLabel      *_signOutLabel,*_statusLabel;
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
    void timerUpdate();

};

#endif // CFUNCTIONTESTVEHICLEVOLTAGEWINDOW_H
