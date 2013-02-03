#ifndef CFULLTESTKEYWINDOW_H
#define CFULLTESTKEYWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QtGui>
#include <QGridLayout>
#include "configure/cmeterconfig.h"

class CFullTestKeyWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFullTestKeyWindow(QWidget *parent = 0);

public:
    //按键名称F1,F2,F3,F4
    QLabel      *_f1Label,*_f2Label,*_f3Label,*_f4Label,*_iLabel;
    QLabel      *_statusLabel;

    //标准按键值
    QLabel      *_f1KeyValue;
    QLabel      *_f2KeyValue;
    QLabel      *_f3KeyValue;
    QLabel      *_f4KeyValue;

    //当前按键
    QLabel      *_fCurrentKeyValue;
    QGroupBox   *_mainGroup,*_resultGroup;
    CMeterConfig           *cf;

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void sendBackData(QByteArray);   
};

#endif
