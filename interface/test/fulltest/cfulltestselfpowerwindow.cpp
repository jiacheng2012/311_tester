#include "cfulltestselfpowerwindow.h"
#include "protocol/protocol.h"
#include "capp.h"

CFullTestSelfPowerWindow::CFullTestSelfPowerWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);
    setProperty("index",0);

    //***UI components
    _statusLabel = new QLabel(tr("点击测试按钮开始仪表自供电测试，请观察仪表是否在主电，常电切换过程中是否工作正常。"));
    _statusLabel->setStyleSheet("font:bold 14px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");
    _testResult = new QLabel(tr("...自供电状态..."));
    _testResult->setStyleSheet("background:wheat;min-width:240px;min-height:30px;border:1px solid blue;");
    _testResult->setAlignment(Qt::AlignCenter);
    _startButton = new QPushButton(tr("开始自供电测试"));

    //***Layout
    QVBoxLayout *globalVLayout = new QVBoxLayout;
    QGroupBox *_mainGroupBox =new QGroupBox(tr("自供电测试"));
    QVBoxLayout *_vl = new QVBoxLayout(_mainGroupBox);
    _vl->addWidget(_statusLabel);
    QGridLayout *grid = new QGridLayout();

    _label1 = new QLabel(tr(" 主电开 "));
    _label1->setStyleSheet("font:bold 18px;color:green;max-height:30px;min-height:30px;background:#CCFF99;");
    _label2 = new QLabel(tr(" 常电关 "));
    _label2->setStyleSheet("font:bold 18px;color:red;max-height:30px;min-height:30px;background:wheat;");

    grid->addWidget(_label1,0,0);
    grid->addWidget(_label2,0,1);

    grid->addWidget(new QLabel(tr("仪表反馈：")),1,0);
    grid->addWidget(_testResult,1,1);
    grid->addWidget(_startButton,2,1);

    _vl->addLayout(grid);
    globalVLayout->addWidget(_mainGroupBox);
    setLayout(globalVLayout);

    //***signal
    connect(_startButton,SIGNAL(clicked()),this,SLOT(startButtonClicked()));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_selfpower(QByteArray)),this,SLOT(sendBackData(QByteArray)));

    _timer = new QTimer;
    connect(_timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFullTestSelfPowerWindow::timerUpdate()
{
    //***PLC
    if(property("index").toInt()>=4)
        setProperty("index",0);

    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    if(property("index").toInt()==1) //*** 主电开，常电开
    {
        c._12v = 0x1;
        c._xxv = 0x2;
        _label1->setText(tr(" 主电开 "));
        _label2->setText(tr(" 常电开 "));
        _label1->setStyleSheet("font:bold 18px;color:green;max-height:30px;min-height:30px;background:#CCFF99;");
        _label2->setStyleSheet("font:bold 18px;color:green;max-height:30px;min-height:30px;background:#CCFF99;");
    }
    else if(property("index").toInt()==2) //*** 主电关，常电开
    {
        c._12v = 0x1;
        c._xxv = 0x0;
        _label1->setText(tr(" 主电关 "));
        _label2->setText(tr(" 常电开 "));
        _label1->setStyleSheet("font:bold 18px;color:red;max-height:30px;min-height:30px;background:wheat;");
        _label2->setStyleSheet("font:bold 18px;color:green;max-height:30px;min-height:30px;background:#CCFF99;");
    }
    else if(property("index").toInt()==3) //*** 主电开，常电开
    {
        c._12v = 0x1;
        c._xxv = 0x2;
        _label1->setText(tr(" 主电开 "));
        _label2->setText(tr(" 常电开 "));
        _label1->setStyleSheet("font:bold 18px;color:green;max-height:30px;min-height:30px;background:#CCFF99;");
        _label2->setStyleSheet("font:bold 18px;color:green;max-height:30px;min-height:30px;background:#CCFF99;");
    }
    else if(property("index").toInt()==0) //*** 主电开，常电关
    {
        c._12v = 0x0;
        c._xxv = 0x2;
        _label1->setText(tr(" 主电开 "));
        _label2->setText(tr(" 常电关 "));
        _label1->setStyleSheet("font:bold 18px;color:green;max-height:30px;min-height:30px;background:#CCFF99;");
        _label2->setStyleSheet("font:bold 18px;color:red;max-height:30px;min-height:30px;background:wheat;");
    }

    setProperty("index",property("index").toInt()+1);

    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
}

void CFullTestSelfPowerWindow::startButtonClicked()
{
    setProperty("index",0);
    if(!property("testFlag").toBool())
    {
        _timer->start(_MS_3000_);
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[1],5);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

         _startButton->setText(tr("停止自供电测试"));
        emit newMessage(tr("开始自供电测试"),0);
        setProperty("testFlag", 1);
    }
    else
    {
        _timer->stop();
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        c._12v = 0x0;
        c._xxv = 0x2;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        _label1->setText(tr(" 主电开 "));
        _label2->setText(tr(" 常电关 "));
        _label1->setStyleSheet("font:bold 18px;color:green;max-height:30px;min-height:30px;background:#CCFF99;");
        _label2->setStyleSheet("font:bold 18px;color:red;max-height:30px;min-height:30px;background:wheat;");

        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[1],5);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);
        _startButton->setText(tr("开始自供电测试"));
        setProperty("testFlag",0);

        _testResult->setText(tr("仪表自供电关闭"));
        _testResult->setStyleSheet("background:red;");

        emit newMessage(tr("结束自供电测试"),0);
    }
}

void CFullTestSelfPowerWindow::sendBackData(QByteArray a)
{
    hardwareTestRespondFrame_0x20 rt = frame20FromData(a);

    if(property("testFlag").toBool())
    {
        if(getBit(rt.control,0))
        {
            _testResult->setText(tr("仪表自供电打开"));
            _testResult->setStyleSheet("background:green;");
        }
        else
        {
            _testResult->setText(tr("仪表自供电关闭"));
            _testResult->setStyleSheet("background:red;");
        }
    }
}
