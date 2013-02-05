#ifndef CFULLTESTIOINWINDOW_H
#define CFULLTESTIOINWINDOW_H

#include <QWidget>
#include <QtGui>
#include <protocol/protocol.h>
#include "configure/cmeterconfig.h"

class CFullTestIOInWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFullTestIOInWindow(QWidget *parent = 0);

public:
    int _ioItemCount ; //总共32路

    QVector<QLabel *> _vec32IO; //32路IO 圆形指示
    QPushButton *_testButton1,*_testButton2;  //32路IO输入量测试
    QCheckBox           *_bReadCurrent;//读取激励电流
    QLabel              *_statusLabel,*_statusLabel_1;

    CMeterConfig           *cf;
    QTimer      *_timer;

    void setMap(PLCCommandFrame_0x80* a, int i, int id=-1);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void testButton1Clicked();
    void testButton2Clicked();
    void sendBackData232(QByteArray);
    void sendBackData485(QByteArray);
};

#endif
