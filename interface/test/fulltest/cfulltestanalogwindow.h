#ifndef CFULLTESTANALOGWINDOW_H
#define CFULLTESTANALOGWINDOW_H

#include <QWidget>
#include <QtGui>
#include "protocol/protocol.h"
#include "configure/cmeterconfig.h"

class CFullTestAnalogWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFullTestAnalogWindow(QWidget *parent = 0);

public:
    QLabel          *_currentItem,*_statusLabel,*_nameLabel,*_statusLabel_0;//当前调校指示
    QSpinBox        *_channelSample;//采样通道
    QComboBox       *_analogType; //模拟量测试类型 1.电流 4~20mA 2.电阻 0~200欧
    QSpinBox        *_actualSampleValue[_MAX_ATEST_POINT_];//模拟量实际值
    QLabel          *_dashboardSampleValue[_MAX_ATEST_POINT_];//仪表板采样值
    QLabel          *_theorySampleValue[_MAX_ATEST_POINT_];//理论采样值
    QPushButton     *_startButton[_MAX_ATEST_POINT_];//理论采样值

    QGridLayout     *_resultGridLayout;

    QPushButton     *_stopButton;

    int             _id,_channel,_type;
    CMeterConfig    *cf;

    void setMap(PLCCommandFrame_0x80* a,int val);
    int findChannelIndex(int t,int c);
    bool findMeterVoltageSampleChannel(int,int);
    QByteArray setupLoader(int val,int type,int channel);
    void startTest();

    QPushButton     *_prvButton,*_nxtButton;

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void startButtonClicked();
    void channelChanged(int);
    void sendBackData485(QByteArray);
    void sendBackData232(QByteArray);
    void actualValueChanged(int index);
    void stopButtonClicked();
    void prvButtonClicked();
    void nxtButtonClicked();
    
};

#endif
