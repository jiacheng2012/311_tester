#ifndef CFUNCTIONTESTLEDWINDOW_H
#define CFUNCTIONTESTLEDWINDOW_H

#include <QWidget>
#include <QtGui>
#include "protocol/protocol.h"
#include "configure/cmeterconfig.h"

class CFunctionTestLEDWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFunctionTestLEDWindow(QWidget *parent = 0);

private:
    QLabel *_ledStateLabels[BITMAP_NAME_COUNT];
    QLabel *_ledStateBitmaps[BITMAP_NAME_COUNT];

    QComboBox *_clampComboBox;//小灯开关信号
    QComboBox *_frontFoglampComboBox;//前雾灯开关信号
    QComboBox *_wipersComboBox;//雨刮开关信号
    QComboBox *_dashboardPowerComboBox;//仪表电源开关信号
    QComboBox *_onComboBox;//ON档开关信号
    QComboBox *_accComboBox;//ACC档开关信号

    QLabel *_lateralOutLabel; //侧标输出
    QLabel *_wipersOutLabel; //雨刮输出
    QLabel *_powerOutLabel; //电源输出
    QLabel *_statusLabel;


    QPushButton *_startButton;
    QTimer      *_timer;
    int         _cur;
    CMeterConfig           *cf;

    void setIOMap(PLCCommandFrame_0x80* a,int i);

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void startButtonClicked();
    void timerUpdate();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

    
};

#endif
