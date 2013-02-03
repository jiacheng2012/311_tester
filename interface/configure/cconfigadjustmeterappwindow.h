#ifndef CCONFIGADJUST_CHANNEL_WINDOW_H
#define CCONFIGADJUST_CHANNEL_WINDOW_H

#include <QWidget>
#include <QtGui>

class CConfigAdjustMeterAppWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CConfigAdjustMeterAppWindow(QWidget *parent = 0);

private:

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

    QPushButton* _saveButton;
    QPushButton* _readButton; //读取配置文件
signals:
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
public slots:
    void    on__readButton_clicked();
    void    on__saveButton_clicked();
};

#endif // CCONFIGADJUST_CHANNEL_WINDOW_H
