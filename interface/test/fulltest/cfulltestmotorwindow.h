#ifndef CFULLTESTMOTORWINDOW_H
#define CFULLTESTMOTORWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

class CFullTestMotorWindow : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE CFullTestMotorWindow(QWidget *parent = 0);

private:

    //步进电机 0，1，2，3，清零 按钮
    QPushButton *_motorButton0;
    QPushButton *_motorButton1;
    QPushButton *_motorButton2;
    QPushButton *_motorButton3;
    QPushButton *_motorZeroButton;
    QPushButton *_motorAllButton;
    QPushButton *_nextButton; //下一步
    CMeterConfig           *cf;
    QLabel      *_statusLabel;

signals:
    void sendData(QByteArray,int);
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void sendBackData(QByteArray);
    void motorButton0Clicked();
    void motorButton1Clicked();
    void motorButton2Clicked();
    void motorButton3Clicked();
    void motorZeroButtonClicked();
    void motorAllButtonClicked();
};

#endif
