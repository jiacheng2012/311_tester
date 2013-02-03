#ifndef CFULLTESTLEDWINDOW_H
#define CFULLTESTLEDWINDOW_H

#include <QWidget>
#include <QtGui>
#include "protocol/protocol.h"
#include "configure/cmeterconfig.h"

class CFullTestLEDWindow : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE CFullTestLEDWindow(QWidget *parent = 0);
public:
    QLabel *_ledStateLabels[BITMAP_NAME_COUNT];
    QLabel *_ledStateBitmaps[BITMAP_NAME_COUNT];

    QPushButton *_startButton;
    QGroupBox   *_mainGroup;
    CMeterConfig           *cf;
    QLabel      *_statusLabel;

signals:
    void sendData(QByteArray,int);
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void sendBackData(QByteArray);
    void startButtonClicked();
};

#endif
