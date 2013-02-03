#ifndef CFULLTESTIOOUTWINDOW_H
#define CFULLTESTIOOUTWINDOW_H

#include <QWidget>
#include <QtGui>
#include "protocol/protocol.h"
#include "configure/cmeterconfig.h"

class CFullTestIOOutWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFullTestIOOutWindow(QWidget *parent = 0);

public:
    int _itemCount;
    QVector<QLabel*> _dashboardMark;
    QVector<QLabel*> _plcMark;
    QPushButton *_testButton;
    QLabel      *_statusLabel;
    CMeterConfig           *cf;

    void setMap(PLCCommandFrame_0x80* a,int i);
    bool getMap(PLCCResponseFrame_0x81 a,int i);

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void sendBackData232(QByteArray a);
    void sendBackData485(QByteArray a);
    void testButtonClicked();
};

#endif
