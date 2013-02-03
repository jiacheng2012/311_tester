#ifndef CFUNCTIONTESTFUELOILTABLEWINDOW_H
#define CFUNCTIONTESTFUELOILTABLEWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

/**************
 *燃油表测试
*************/

class CFunctionTestFuelOilTableWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFunctionTestFuelOilTableWindow(QWidget *parent = 0);

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
