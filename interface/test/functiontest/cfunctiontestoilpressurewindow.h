#ifndef CFUNCTIONTESTOILPRESSUREWINDOW_H
#define CFUNCTIONTESTOILPRESSUREWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

/**************
 *机油压力测试
*************/

class CFunctionTestOilPressureWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFunctionTestOilPressureWindow(QWidget *parent = 0);

public:
    QSpinBox    *_currentValue;
    QPushButton *_autoTestButton;
    QPushButton *_singleTestButton;
    QLabel      *_signOutLabel,*_statusLabel;
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
