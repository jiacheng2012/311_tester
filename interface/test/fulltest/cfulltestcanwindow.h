#ifndef CFULLTESTCANWINDOW_H
#define CFULLTESTCANWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

class CFullTestCanWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFullTestCanWindow(QWidget *parent = 0);

public:
    QLineEdit   *_canSendDataID,*_canSendDataData;
    QLineEdit   *_canRecieveData0,*_canSendData0;
    QLineEdit   *_canRecieveData1,*_canSendData1;
    QPushButton *_testButton;

    CMeterConfig           *cf;
    QLabel      *_statusLabel,*_can0RNum,*_can1RNum,*_can0SNum,*_can1SNum;
    QPushButton  *_can0ZeroButton,*_can1ZeroButton;

signals:
    void nextList(int);
    void newMessage(QString msg, bool showOnScreen=0);

public slots:
    void testButtonClicked();
    void sendBackData232(QByteArray a);
    void sendBackData485(QByteArray a);
    void can0ZeroButtonClicked();
    void can1ZeroButtonClicked();
};

#endif
