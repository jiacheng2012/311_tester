#ifndef CFUNCTIONTESTSPEEDTABLEWINDOW_H
#define CFUNCTIONTESTSPEEDTABLEWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"
#include "protocol/protocol.h"

class CFunctionTestSpeedTableWindow : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE CFunctionTestSpeedTableWindow(QWidget *parent = 0);

public:
    QSpinBox    *_currentValue;
    QPushButton *_autoTestButton;
    QPushButton *_singleTestButton;
    QLabel      *_signOutLabel,*_statusLabel;
    int         _cur;
    QTimer      *_timer;
    CMeterConfig           *cf;

    void setMap(PLCCommandFrame_0x80* a,int id, int label);

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void autoTestButtonClicked();
    void singleTestButtonClicked();
    void timerUpdate();

};

#endif // CFUNCTIONTESTSPEEDTABLEWINDOW_H
