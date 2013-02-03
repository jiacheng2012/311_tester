#include "cfulltestlcdwindow.h"
#include "protocol/protocol.h"
#include "capp.h"

CFullTestLCDWindow::CFullTestLCDWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);
    //***UI components
    _statusLabel = new QLabel(tr("点击测试，并且观察仪表液晶屏自检是否正常。"));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");
    _startButton = new QPushButton(tr("开始液晶屏测试"));
    _startButton->setStyleSheet("min-width:180px;min-height:26px;");

    _statusLabel = new QLabel("请观察液晶屏是否执行测试内容");
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    //***Layout
    QVBoxLayout* top = new QVBoxLayout;

    QGroupBox *mainGroup = new QGroupBox(tr("液晶屏测试"), this);
    QVBoxLayout *mainVLayout = new QVBoxLayout(mainGroup);

    mainVLayout->addWidget(_statusLabel, 0, Qt::AlignCenter);
    mainVLayout->addWidget(_startButton, 0, Qt::AlignCenter);
    mainVLayout->setSpacing(20);
    mainVLayout->setContentsMargins(15, 20, 15, 20);

    top->addWidget(mainGroup, 0, Qt::AlignLeft | Qt::AlignTop);
    setLayout(top);
    //***signals
    connect(_startButton,SIGNAL(clicked()),this,SLOT(startButtonClicked()));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFullTestLCDWindow::startButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[1],4);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _startButton->setText(tr("停止液晶屏测试"));
        emit newMessage(tr("开始液晶屏测试"),0);
        setProperty("testFlag",1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[1],4);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _startButton->setText(tr("开始液晶屏测试"));
        setProperty("testFlag",0);
    }
}
void CFullTestLCDWindow::sendBackData(QByteArray a)
{

}
