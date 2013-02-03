#include "cfulltestmotorwindow.h"
#include "protocol/protocol.h"
#include "capp.h"

CFullTestMotorWindow::CFullTestMotorWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);

    //*** UI CONPONENTS
    _statusLabel = new QLabel(tr("请观察步进电机是否按照指令动作,请注意执行归零命令后，要及时停止该动作。"));
    _statusLabel->setStyleSheet("font:bold 14px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    _motorButton0 = new QPushButton("开始电机0(油温)归零测试");
    _motorButton1 = new QPushButton("开始电机1(转速)归零测试");
    _motorButton2 = new QPushButton("开始电机2(水温)归零测试");
    _motorButton3 = new QPushButton("开始电机3(车速)归零测试");
    _motorZeroButton = new QPushButton("开始所有电机归零测试");
    _motorAllButton = new QPushButton("开始步进电机整体测试");

    _motorButton0->setProperty("page","fulltest-topbtn");
    _motorButton1->setProperty("page","fulltest-topbtn");
    _motorButton2->setProperty("page","fulltest-topbtn");
    _motorButton3->setProperty("page","fulltest-topbtn");
    _motorZeroButton->setProperty("page","fulltest-topbtn");
    _motorAllButton->setProperty("page","fulltest-topbtn");

    //*** Layout
    QVBoxLayout *_mainLayout = new QVBoxLayout();
    QGroupBox  *_mainGroupBox = new QGroupBox(tr("步进电机测试"),this);
    QGridLayout *_mainGrid = new QGridLayout;
    QVBoxLayout *_vl = new QVBoxLayout(_mainGroupBox);
    _vl->addWidget(_statusLabel);
    _mainGrid->addWidget(_motorAllButton,0,0);
    _mainGrid->addWidget(_motorZeroButton,0,1);
    _mainGrid->addWidget(_motorButton0,1,0);
    _mainGrid->addWidget(_motorButton1,1,1);
    _mainGrid->addWidget(_motorButton2,2,0);
    _mainGrid->addWidget(_motorButton3,2,1);

    _mainGrid->setContentsMargins(20,15,20,15);
    _mainGrid->setSpacing(20);

    _vl->addLayout(_mainGrid);
    _mainLayout->addWidget(_mainGroupBox,0, Qt::AlignLeft | Qt::AlignTop);
    _mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(_mainLayout);
    //***Signal
     connect(_motorButton0,SIGNAL(clicked()),this,SLOT(motorButton0Clicked()));
     connect(_motorButton1,SIGNAL(clicked()),this,SLOT(motorButton1Clicked()));
     connect(_motorButton2,SIGNAL(clicked()),this,SLOT(motorButton2Clicked()));
     connect(_motorButton3,SIGNAL(clicked()),this,SLOT(motorButton3Clicked()));
     connect(_motorZeroButton,SIGNAL(clicked()),this,SLOT(motorZeroButtonClicked()));
     connect(_motorAllButton,SIGNAL(clicked()),this,SLOT(motorAllButtonClicked()));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFullTestMotorWindow::motorButton0Clicked()
{
    if(!property("testFlag").toBool())
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[0],2);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorButton0->setText(tr("停止电机0(油温)归零测试"));
        emit newMessage(tr("开始电机0(油温)归零测试"),0);
        setProperty("testFlag",1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[0],2);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorButton0->setText(tr("开始电机0(油温)归零测试"));
        setProperty("testFlag",0);
    }
}

void CFullTestMotorWindow::motorButton1Clicked()
{
    if(!property("testFlag").toBool())
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[0],3);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorButton1->setText(tr("停止电机1(转速)归零测试"));
        emit newMessage(tr("开始电机1(转速)归零测试"),0);
        setProperty("testFlag",1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[0],3);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorButton1->setText(tr("开始电机1(转速)归零测试"));
        setProperty("testFlag",0);
    }
}

void CFullTestMotorWindow::motorButton2Clicked()
{
    if(!property("testFlag").toBool())
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[0],4);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorButton2->setText(tr("停止电机2(水温)归零测试"));
        emit newMessage(tr("开始电机2(水温)归零测试"),0);
        setProperty("testFlag",1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[0],4);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorButton2->setText(tr("开始电机2(水温)归零测试"));
        setProperty("testFlag",0);
    }
}

void CFullTestMotorWindow::motorButton3Clicked()
{
    if(!property("testFlag").toBool())
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[0],5);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorButton3->setText(tr("停止电机3(车速)归零测试"));
        emit newMessage(tr("开始电机3(车速)归零测试"),0);
        setProperty("testFlag",1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[0],5);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorButton3->setText(tr("开始电机3(车速)归零测试"));
        setProperty("testFlag",0);
    }
}

void CFullTestMotorWindow::motorZeroButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[0],2);
        setBit(b.command[0],3);
        setBit(b.command[0],4);
        setBit(b.command[0],5);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorZeroButton->setText(tr("停止所有电机归零测试"));
        emit newMessage(tr("开始所有电机归零测试"),0);
        setProperty("testFlag",1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[0],2);
        clearBit(b.command[0],3);
        clearBit(b.command[0],4);
        clearBit(b.command[0],5);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorZeroButton->setText(tr("开始所有步进电机归零测试"));
        setProperty("testFlag",0);
    }
}

void CFullTestMotorWindow::motorAllButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[0],6);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorAllButton->setText(tr("停止步进电机整体测试"));
        emit newMessage(tr("开始步进电机整体测试"),0);
        setProperty("testFlag",1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[0],6);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _motorAllButton->setText(tr("开始步进电机整体测试"));
        setProperty("testFlag",0);
    }
}

void CFullTestMotorWindow::sendBackData(QByteArray a)
{

}
