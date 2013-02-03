#ifndef CADJUSTTESTANALOGWINDOW_H
#define CADJUSTTESTANALOGWINDOW_H

#include <QWidget>
#include <QtGui>
#include "protocol/protocol.h"
#include "protocol/cdataframe.h"
#include "configure/cadjustconfig.h"
#include "configure/cmeterconfig.h"

class CAdjustTestAnalogWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CAdjustTestAnalogWindow(QWidget *parent = 0);

public:
    QLabel          *_currentItem,*_statusLabel;//当前调校指示
    QSpinBox        *_channelSample;//采样通道
    QComboBox       *_analogType; //模拟量测试类型 1.电流 4~20mA 2.电阻 0~200欧
    QSpinBox        *_actualValue[_MAX_AADJUST_POINT_];//模拟量实际值
    QLabel          *_sampleValue[_MAX_AADJUST_POINT_];//仪表板采样值
    QLabel          *_savedSampleValue[_MAX_AADJUST_POINT_];//仪表板采样值
    QLabel          *_theorySampleValue[_MAX_AADJUST_POINT_];//理论采样值
    QPushButton     *_startButton[_MAX_AADJUST_POINT_];//理论采样值
    QPushButton     *_savaAllAdjustButton,*_readButton,*_writeButton;
    QGridLayout     *_resultGridLayout;
    QLabel          *_savaStatusLabel,*_statusLabel_0;

    QPushButton     *_stopButton;

    CAdjustConfig    *cf;
    CMeterConfig     *mf;
    QPushButton     *_prvButton,*_nxtButton;

    void    setMap(PLCCommandFrame_0x80* a,int val);
    int     findChannelIndex(int t,int c);
    int     findChannelIndexReverse(int t,int c);
    void    updateInterface(int number,int type,int channel,vu16 value,vu16 sample);
    void    sendFrame(int,int);
    void    writeAdjustInitFrame(adjustInitParaFrame_0x41 &b);
    bool findMeterVoltageSampleChannel(int,int);
    QByteArray setupLoader(int val,int type,int channel);
    void    startTest();

signals:
    void    newMessage(QString msg, bool showOnScreen=0);
    void    nextList(int);
protected:
    void keyReleaseEvent(QKeyEvent *event);
public slots:
    void startButtonClicked();
    void channelChanged(int);
    void sendBackData485(QByteArray);
    void sendBackData232(CDataFrame);
    void actualValueChanged(int index);
    void stopButtonClicked();
    void writeButtonClicked();
    void readButtonClicked();
    void saveAllAdjustButtonClicked();
    void prvButtonClicked();
    void nxtButtonClicked();
};

#endif
