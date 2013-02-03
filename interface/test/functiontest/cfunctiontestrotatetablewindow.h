#ifndef CFUNCTIONTESTROTATETABLEWINDOW_H
#define CFUNCTIONTESTROTATETABLEWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

/**************
 *转速表测试
*************/

class CFunctionTestRotateTableWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFunctionTestRotateTableWindow(QWidget *parent = 0);

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
