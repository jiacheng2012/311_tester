#ifndef CFUNCTIONTESTSETTINGWINDOW_H
#define CFUNCTIONTESTSETTINGWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"
#include "protocol/cdataframe.h"

/**************
 *功能全检设置
*************/

class CFunctionTestSettingWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFunctionTestSettingWindow(QWidget *parent = 0);

public:
    QLabel      *_tip;
    QSpinBox* _accumulatedMileage; //累计里程
    QSpinBox* _simpleMileage; //小计里程

    QSpinBox* _ledlight; //背光亮度
    QSpinBox* _lcdbacklight; //指示灯亮度

    QSpinBox* _lowspeedFanDiff; //低速风扇回差
    QSpinBox* _highspeedFanDiff; //高速风扇回差

    QSpinBox* _warn_coolingWaterTemperature; //冷却水温报警点
    QSpinBox* _warn_oilpressure; //机油压力报警点
    QSpinBox* _warn_oil; //燃油报警点
    QSpinBox* _warn_lowpressure; //气压低报警点

    QSpinBox* _lowspeedFanInGasTemperature; //低速风扇进气温度参数
    QSpinBox* _highspeedFanInGasTemperature; //高速风扇进气温度参数
    QSpinBox* _lowspeedFan_coolingWaterTemperature; //低速风扇冷却水温参数
    QSpinBox* _highspeedFan_coolingWaterTemperature; //高速风扇冷却水温参数

    QSpinBox* _pulseCount; //脉冲计数
    QSpinBox* _engineWorkTime_Hour; //发动机工作时间(时)
    QSpinBox* _engineWorkTime_Minute; //发动机工作时间(分)
    QSpinBox* _engineWorkTime_Second; //发动机工作时间(秒)

    QPushButton* _readButton,*_writeButton;
    QLabel* _statusLabel;  //下载参数到仪表的状态
    QPushButton* _downloadParmsButton;//下载参数到仪表 按钮

    QSpinBox    *_year,*_month,*_day,*_hour,*_minute,*_second;
    QSpinBox    *_oilBias,*_rspeedBias,*_wtempBias,*_speedBias;
    QComboBox   *_language,*_theme,*_version,*_paramSetting,*_allowZero;
    QComboBox   *_loilType,*_brakeType,*_brakeValid;
    QLineEdit   *_passwd,*_engineType,*_gearType;

    CMeterConfig* cf;


    void updateInterface();

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void    readButtonClicked();
    void    writeButtonClicked();
    void    sendBackTestData_232(CDataFrame);
    void    delayWrite();
};

#endif // CFUNCTIONTESTSETTINGWINDOW_H

