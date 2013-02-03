#include "cfulltestlcdbacklightwindow.h"
#include "protocol/protocol.h"
#include "capp.h"

CFullTestLCDBackLightWindow::CFullTestLCDBackLightWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);

    //***UI components
    _startButton = new QPushButton(tr("开始液晶屏背光测试"));
    _startButton->setStyleSheet("min-width:180px;min-height:26px;");

    //***Layout
    QVBoxLayout* top = new QVBoxLayout;

    QGroupBox *mainGroup = new QGroupBox(tr("液晶屏背光测试"));
    QVBoxLayout *mainVLayout = new QVBoxLayout(mainGroup);
    _statusLabel = new QLabel("背光亮度：0%");
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    mainVLayout->addWidget(_statusLabel);
    mainVLayout->addWidget(_startButton, 0, Qt::AlignCenter);
    mainVLayout->setSpacing(20);
    mainVLayout->setContentsMargins(15, 20, 15, 20);

    top->addWidget(mainGroup, 0, Qt::AlignLeft | Qt::AlignTop);
    setLayout(top);
    //***signals
    connect(_startButton,SIGNAL(clicked()),this,SLOT(startButtonClicked()));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_lcdbacklight(QByteArray)),this,SLOT(sendBackData(QByteArray)));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFullTestLCDBackLightWindow::sendBackData(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        hardwareTestRespondFrame_0x20 rt = frame20FromData(a);

        _statusLabel->setText(tr("背光亮度：") + QString::number(rt.lcd_light) + tr("%"));
    }
}

void CFullTestLCDBackLightWindow::startButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[1],2);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

         _startButton->setText(tr("停止液晶屏背光测试"));
        emit newMessage(tr("开始液晶屏背光测试"),0);
        setProperty("testFlag", 1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[1],2);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _startButton->setText(tr("开始液晶屏背光测试"));
        setProperty("testFlag", 0);
    }
}
