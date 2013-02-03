#ifndef CFULLTESTMEMORYWINDOW_H
#define CFULLTESTMEMORYWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

class CFullTestMemoryWindow : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE CFullTestMemoryWindow(QWidget *parent = 0);


    QLabel      *_testResult;
    QPushButton *_startButton;
    CMeterConfig           *cf;
    QLabel      *_statusLabel;

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void sendBackData(QByteArray);
    void startButtonClicked();
};

#endif
