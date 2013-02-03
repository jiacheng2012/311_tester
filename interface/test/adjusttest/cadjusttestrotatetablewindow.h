#ifndef CADJUSTTESTROTATETABLEWINDOW_H
#define CADJUSTTESTROTATETABLEWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cadjustconfig.h"
#include "protocol/protocol.h"
#include "protocol/cdataframe.h"

/**************
 *转速表调校
*************/

class CAdjustTestRotateTableWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CAdjustTestRotateTableWindow(QWidget *parent = 0);

public:
    QLabel  *_currentItem;//当前调校指示

    QSpinBox*               _currentStep[_MAX_MADJUST_POINT_];   //现在微步数
    QLabel*        _theoryValue[_MAX_MADJUST_POINT_];   //理论值
    QLabel*        _sampleValue[_MAX_MADJUST_POINT_];   //采样值
    QPushButton*   _adjustButton[_MAX_MADJUST_POINT_];  //调校按钮

    QLabel      *_statusLabel;
    QSpinBox    *_currentAmount;//当前微步数
    QPushButton *_zeroButton;
    QPushButton *_increaseButton;//增加微步数
    QPushButton *_decreaseButton;//减少微步数
    QPushButton *_savaAllAdjustButton;//保存所有调校
    QPushButton *_writeButton;
    QPushButton *_readButton;

    QLabel *_savaStatusLabel;//保存调校完成状态显示 label

    QGroupBox   *_resultGroup;
    QGridLayout *_centerGrid;

    CAdjustConfig           *cf;

    void sendFrame(quint8 c,quint8 m,qint16 s);
    void updateInterface(int number,vu16 value,vu16 sample);
    void writeAdjustInitFrame(adjustInitParaFrame_0x41 &b);

signals:
    void newMessage(QString,bool);
    void nextList(int);
protected:
    void keyReleaseEvent(QKeyEvent *event);
public slots:
    void zeroButtonClicked();
    void increaseButtonClicked();
    void decreaseButtonClicked();
    void sendBackData(CDataFrame d);

    void stepValueChanged(int);
    void adjustButtonClicked();

    void saveAllAdjustButtonClicked();
    void writeButtonClicked();
    void readButtonClicked();
};

#endif
