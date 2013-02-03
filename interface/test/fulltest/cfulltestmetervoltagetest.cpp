#include "cfulltestmetervoltagewindow.h"
#include "capp.h"
#include "protocol/protocol.h"

CFullTestMeterVoltageWindow::CFullTestMeterVoltageWindow(QWidget *parent) :
    QWidget(parent)
{
    //***UI components
    setProperty("testFlag",0);
    _val = 0x2;
    _statusLabel = new QLabel(tr("选择仪表的不同电源电压值，然后观察仪表自采样电压和信号箱采样值。"));
    _statusLabel->setStyleSheet("font:bold 14px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    _result1 = new QLabel();
    _result1->setStyleSheet("font:bold 16px;color:blue;max-height:26px;min-height:26px;background:#CCFF99;");
    _result2 = new QLabel();
    _result2->setStyleSheet("font:bold 16px;color:blue;max-height:26px;min-height:26px;background:#CCFF99;");
    _startButton = new QPushButton(tr("开始电源电压采样测试"));

    _voltageBox = new QComboBox();
    _voltageBox->addItem(tr("12V"));
    _voltageBox->addItem(tr("24V"));
    _voltageBox->addItem(tr("36V"));
    _voltageBox->setCurrentIndex(1);

    //***Layout
    QVBoxLayout *globalVLayout = new QVBoxLayout;

    //***Layout
    QGroupBox *_mainGroup = new QGroupBox(tr("仪表电源电压采样"),this);
    QVBoxLayout *_vl = new QVBoxLayout(_mainGroup);
    _vl->addWidget(_statusLabel);
    QGridLayout *_mainGrid =  new QGridLayout;
    _mainGrid->addWidget(new QLabel(tr("仪表供电电压值")),0,0);
    _mainGrid->addWidget(_voltageBox,0,1);

    _mainGrid->addWidget(new QLabel(tr("仪表自采样电压值")),1,0);
    _mainGrid->addWidget(_result1,1,1);
    _mainGrid->addWidget(new QLabel(tr("信号箱采样电压值")),2,0);
    _mainGrid->addWidget(_result2,2,1);
    _mainGrid->addWidget(_startButton,3,1);

    _vl->addLayout(_mainGrid);
    globalVLayout->addWidget(_mainGroup);
    globalVLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(globalVLayout);

    //***signal
    connect(_startButton,SIGNAL(clicked()),this,SLOT(startButtonClicked()));
    connect(_voltageBox,SIGNAL(currentIndexChanged(int)),this,SLOT(indexChanged(int)));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_metervoltage232(QByteArray)),this,SLOT(sendBackData232(QByteArray)));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_metervoltage485(QByteArray)),this,SLOT(sendBackData485(QByteArray)));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFullTestMeterVoltageWindow::startButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        //***打开常电，切换到当前选择的电压
        c._12v = 0x1;
        c._xxv = _val;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        _startButton->setText(tr("停止仪表电源电压采样测试"));
        emit newMessage(tr("开始仪表电源采样测试"),0);
        setProperty("testFlag", 1);
    }
    else
    {
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        //***关闭常电，切换到24V
        c._12v = 0x0;
        c._xxv = 0x2;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        _startButton->setText(tr("开始仪表供电电压采样"));
        setProperty("testFlag",0);
    }
}

void CFullTestMeterVoltageWindow::indexChanged(int id)
{
    _val = id+1;
    if(property("testFlag").toBool())
    {
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        //***切换到当前选择的电压
        c._12v = 0x1;
        c._xxv = _val;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
}

void CFullTestMeterVoltageWindow::sendBackData485(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        PLCCResponseFrame_0x81 rt = frame81FromData(a);
        _result2->setText(QString::number(rt.vin[cf->_map_meter_voltage]*0.1*12) + "V");
    }
}

void CFullTestMeterVoltageWindow::sendBackData232(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        hardwareTestRespondFrame_0x20 rt = frame20FromData(a);
        _result1->setText(QString::number(rt.ao[9]*0.01) + "V");
    }
}
