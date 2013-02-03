#include "cfulltestcanwindow.h"
#include "protocol/protocol.h"
#include "capp.h"

CFullTestCanWindow::CFullTestCanWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);

    //***ui component
    QHBoxLayout *top = new QHBoxLayout;

    _statusLabel = new QLabel(tr("点击按钮测试仪表CAN总线收发是否正常。"));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    _can0ZeroButton = new QPushButton(tr("CAN0计数器清零"));
    _can1ZeroButton = new QPushButton(tr("CAN1计数器清零"));
    _canSendDataID = new QLineEdit(tr("01 02 03 04"));
    _canSendDataID->setMaxLength(11);
    _canSendDataData = new QLineEdit(tr("F1 F2 F3 F4 F5 F6 F7 F8"));
    _canSendDataData->setMaxLength(23);
    _canSendDataData->setFixedWidth(200);
    _canRecieveData0 = new QLineEdit();
    _canRecieveData0->setMaxLength(23);
    _canRecieveData0->setFixedWidth(200);
    _canRecieveData1 = new QLineEdit();
    _canRecieveData1->setMaxLength(23);
    _canRecieveData1->setFixedWidth(200);
    _canSendData0 = new QLineEdit();
    _canSendData0->setMaxLength(23);
    _canSendData0->setFixedWidth(200);
    _canSendData1 = new QLineEdit();
    _canSendData1->setMaxLength(23);
    _canSendData1->setFixedWidth(200);

    _can0RNum = new QLabel("0");
    _can1RNum = new QLabel("0");
    _can0SNum = new QLabel("0");
    _can1SNum = new QLabel("0");

    _testButton = new QPushButton(tr("开始CAN测试"));
    _testButton->setFocusPolicy(Qt::NoFocus);

    //***layout
    QGroupBox *_mainGroup = new QGroupBox(tr("CAN测试"));
    QGridLayout *_mainGrid =  new QGridLayout;
    QVBoxLayout *_vl = new QVBoxLayout(_mainGroup);
    _vl->addWidget(_statusLabel);

    _mainGrid->addWidget(new QLabel(tr("接受/发送数")),0,2);
    _mainGrid->addWidget(new QLabel(tr("待发送CAN数据ID：")),1,0);
    _mainGrid->addWidget(_canSendDataID,1,1);
    _mainGrid->addWidget(new QLabel(tr("待发送CAN数据：")),2,0);
    _mainGrid->addWidget(_canSendDataData,2,1);

    _mainGrid->addWidget(new QLabel(tr("仪表CAN0发送数据：")),3,0);
    _mainGrid->addWidget(_canSendData0,3,1);
    _mainGrid->addWidget(_can0SNum,3,2);


    _mainGrid->addWidget(new QLabel(tr("仪表CAN0接受数据：")),4,0);
    _mainGrid->addWidget(_canRecieveData0,4,1);
    _mainGrid->addWidget(_can0RNum,4,2);

    _mainGrid->addWidget(_can0ZeroButton,5,1);

    _mainGrid->addWidget(new QLabel(tr("仪表CAN1接受数据：")),6,0);
    _mainGrid->addWidget(_canRecieveData1,6,1);
    _mainGrid->addWidget(_can1RNum,6,2);

    _mainGrid->addWidget(new QLabel(tr("仪表CAN1发送数据：")),7,0);
    _mainGrid->addWidget(_canSendData1,7,1);
    _mainGrid->addWidget(_can1SNum,7,2);

    _mainGrid->addWidget(_can1ZeroButton,8,1);

    _mainGrid->addWidget(_testButton,9,1);
    _mainGrid->setSpacing(10);
    _mainGrid->setContentsMargins(5,10,0,10);

    _vl->addLayout(_mainGrid);
    top->addWidget(_mainGroup,0, Qt::AlignLeft | Qt::AlignTop);
    setLayout(top);

    //***signal
    connect(_testButton,SIGNAL(clicked()),SLOT(testButtonClicked()));
    connect(_can0ZeroButton,SIGNAL(clicked()),SLOT(can0ZeroButtonClicked()));
    connect(_can1ZeroButton,SIGNAL(clicked()),SLOT(can1ZeroButtonClicked()));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_can_485(QByteArray)),this,SLOT(sendBackData485(QByteArray)));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_can_232(QByteArray)),this,SLOT(sendBackData232(QByteArray)));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFullTestCanWindow::can0ZeroButtonClicked()
{
    if(property("testFlag").toBool())
    {
        int i;
        QByteArray s1=stringToHex(_canSendDataID->text());
        QByteArray s2=stringToHex(_canSendDataData->text());

        //***PLC
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        for(i=0;i<4;i++)
        {
            c.can1_id[i] = s1[2*i]*16 + s1[2*i+1];
            c.can2_id[i] = s1[2*i]*16 + s1[2*i+1];
        }
        for(i=0;i<8;i++)
        {
            c.can1_data[i] = s2[2*i]*16 + s2[2*i+1];
            c.can2_data[i] = s2[2*i]*16 + s2[2*i+1];
        }
        QByteArray tmp = arrangeByte(0xa,2);
        c.can1_rate[0] = tmp[0];
        c.can1_rate[1] = tmp[1];
        c.can2_rate[0] = tmp[0];
        c.can2_rate[1] = tmp[1];

        c.can1_cmd = 0x1;

        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        c.can1_cmd = 0x0;

        tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
}

void CFullTestCanWindow::can1ZeroButtonClicked()
{
    if(property("testFlag").toBool())
    {
        int i;
        QByteArray s1=stringToHex(_canSendDataID->text());
        QByteArray s2=stringToHex(_canSendDataData->text());

        //***PLC
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        for(i=0;i<4;i++)
        {
            c.can1_id[i] = s1[2*i]*16 + s1[2*i+1];
            c.can2_id[i] = s1[2*i]*16 + s1[2*i+1];
        }
        for(i=0;i<8;i++)
        {
            c.can1_data[i] = s2[2*i]*16 + s2[2*i+1];
            c.can2_data[i] = s2[2*i]*16 + s2[2*i+1];
        }
        QByteArray tmp = arrangeByte(0xa,2);
        c.can1_rate[0] = tmp[0];
        c.can1_rate[1] = tmp[1];
        c.can2_rate[0] = tmp[0];
        c.can2_rate[1] = tmp[1];

        c.can2_cmd = 0x1;

        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        c.can2_cmd = 0x0;

        tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
}

void CFullTestCanWindow::testButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        int i;
        //***仪表
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[0],0);
        setBit(b.command[0],1);

        QByteArray s1=stringToHex(_canSendDataID->text());
        QByteArray s2=stringToHex(_canSendDataData->text());

        for(i=0;i<4;i++)
            b.can_id[i] = s1[2*i]*16 + s1[2*i+1];
        for(i=0;i<8;i++)
            b.can_data[i] = s2[2*i]*16 + s2[2*i+1];

        QByteArray tmp1=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp1);

        //***PLC
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        for(i=0;i<4;i++)
        {
            c.can1_id[i] = s1[2*i]*16 + s1[2*i+1];
            c.can2_id[i] = s1[2*i]*16 + s1[2*i+1];
        }
        for(i=0;i<8;i++)
        {
            c.can1_data[i] = s2[2*i]*16 + s2[2*i+1];
            c.can2_data[i] = s2[2*i]*16 + s2[2*i+1];
        }
        QByteArray tmp = arrangeByte(0xa,2);
        c.can1_rate[0] = tmp[0];
        c.can1_rate[1] = tmp[1];
        c.can2_rate[0] = tmp[0];
        c.can2_rate[1] = tmp[1];

        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        _testButton->setText(tr("停止CAN测试"));
        emit newMessage(tr("开始CAN测试"),0);
        setProperty("testFlag", 1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        newMessage(tr("结束CAN测试"));
        _testButton->setText(tr("开始CAN测试"));
        setProperty("testFlag",0);
    }
}


void CFullTestCanWindow::sendBackData485(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        PLCCResponseFrame_0x81 rt = frame81FromData(a);

        _canSendData0->setText( (const char*)hexToString(rt.can1_data) );
        _canSendData1->setText( (const char*)hexToString(rt.can2_data) );
        _can0SNum->setText(QString::number(rt.can1_num));
        _can1SNum->setText(QString::number(rt.can2_num));
    }
}


void CFullTestCanWindow::sendBackData232(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        hardwareTestRespondFrame_0x20 rt = frame20FromData(a);

        _canRecieveData0->setText( (const char*) hexToString(rt.can0_data) );
        _canRecieveData1->setText( (const char*) hexToString(rt.can1_data) );
        _can0RNum->setText(QString::number(rt.can0_count));
        _can1RNum->setText(QString::number(rt.can1_count));
    }
}
