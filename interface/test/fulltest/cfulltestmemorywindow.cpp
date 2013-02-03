#include "cfulltestmemorywindow.h"
#include "protocol/protocol.h"
#include "capp.h"

CFullTestMemoryWindow::CFullTestMemoryWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);

    //***UI components
    _statusLabel = new QLabel(tr("点击按钮进行铁电存储测试。"));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");
    _testResult = new QLabel(tr("...等待测试..."));
    _testResult->setStyleSheet("background:wheat;min-width:240px;min-height:30px;border:1px solid blue;");
    _testResult->setAlignment(Qt::AlignCenter);
    _startButton = new QPushButton(tr("开始存储测试"));
    _startButton->setStyleSheet("min-height:26px; min-width:110px;");

    //***Layout
    QVBoxLayout *globalVLayout = new QVBoxLayout;
    QGroupBox *_mainGroupBox =new QGroupBox(tr("存储测试"),this);
    QGridLayout *_mainGroupHLayout = new QGridLayout(_mainGroupBox);

    _mainGroupHLayout->addWidget(_statusLabel,0,0,1,3);
    _mainGroupHLayout->addWidget(new QLabel(tr("存储测试结果：")),1,0);
    _mainGroupHLayout->addWidget(_testResult,1,1);
    _mainGroupHLayout->addWidget(_startButton,1,2);
    _mainGroupHLayout->setMargin(10);
    _mainGroupHLayout->setSpacing(15);

    globalVLayout->addWidget(_mainGroupBox,0, Qt::AlignLeft | Qt::AlignTop);
    setLayout(globalVLayout);

    //***signal
    connect(_startButton,SIGNAL(clicked()),this,SLOT(startButtonClicked()));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_memory(QByteArray)),this,SLOT(sendBackData(QByteArray)));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFullTestMemoryWindow::startButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[0],7);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

         _startButton->setText(tr("停止存储测试"));
         _testResult->setText(tr("...等待测试..."));
        emit newMessage(tr("开始存储测试"),0);
        setProperty("testFlag", 1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[0],7);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _startButton->setText(tr("开始存储测试"));
        _testResult->setText(tr("...等待测试..."));
        setProperty("testFlag",0);
    }
}

void CFullTestMemoryWindow::sendBackData(QByteArray a)
{
    hardwareTestRespondFrame_0x20 rt = frame20FromData(a);

    if(property("testFlag").toBool())
    {
        if(!getBit(rt.result,1) && getBit(rt.result,0))
        {
            _testResult->setText(tr("...存储测试中..."));
            _testResult->setStyleSheet("background:red;");
        }
        /*存储测试完成*/
        else if(getBit(rt.result,0) && getBit(rt.result,1))
        {
            if(!getBit(rt.result,3) && getBit(rt.result,2))
            {
                _testResult->setText(tr("测试完成<span style='color:green;font-weight:bold;'> 铁电存储测试正常</span>"));
            }
            else if(getBit(rt.result,2) && getBit(rt.result,3))
            {
                _testResult->setText(tr("测试完成<span style='color:red;font-weight:bold;'> 铁电存储测试错误</span>"));
            }

            hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
            clearBit(b.command[0],7);
            QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
            ((CApp*)qApp)->_tjob->sendTestData232(tmp);
        }
    }
}
