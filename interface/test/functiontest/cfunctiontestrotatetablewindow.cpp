#include "cfunctiontestrotatetablewindow.h"
#include "capp.h"

CFunctionTestRotateTableWindow::CFunctionTestRotateTableWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",false);

   //***UI CONPONENTS
    QVBoxLayout *top = new QVBoxLayout;

    QGroupBox *_mainGroupBox = new QGroupBox(tr("转速表测试"));
    QGridLayout *_mainGrid = new QGridLayout(_mainGroupBox);
    _currentValue = new QSpinBox;
    _currentValue->setMinimum(0);
    _currentValue->setMaximum(5000);
    _autoTestButton = new QPushButton(tr("自动检测"));
    _singleTestButton = new QPushButton(tr("测单点"));
    _signOutLabel = new QLabel;
    _signOutLabel->setStyleSheet("border-radius:20px;background:#fff;border:1px solid #666;");
    _signOutLabel->setFixedSize(40,40);
    _statusLabel = new QLabel(tr("状态："));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    //***Layout
    _mainGrid->addWidget(_statusLabel,0,0,1,3);
    _mainGrid->addWidget(new QLabel(tr("转速信号输出：")),1,0);
    _mainGrid->addWidget(_signOutLabel,1,1);
    _mainGrid->addWidget(new QLabel(tr("转速：")),2,0);
    _mainGrid->addWidget(_currentValue,2,1);
    _mainGrid->addWidget(new QLabel(tr("rpm")),2,2);
    _mainGrid->addWidget(_autoTestButton,3,1);
    _mainGrid->addWidget(_singleTestButton,3,2);

    top->addWidget(_mainGroupBox);
    top->setSizeConstraint(QLayout::SetFixedSize);
    top->setAlignment(Qt::AlignCenter);
    setLayout(top);

    //***signals
    connect(_autoTestButton,SIGNAL(clicked()),this,SLOT(autoTestButtonClicked()));
    connect(_singleTestButton,SIGNAL(clicked()),this,SLOT(singleTestButtonClicked()));

    _timer = new QTimer;
    connect(_timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFunctionTestRotateTableWindow::autoTestButtonClicked()
{
    _cur=0;
    if(!property("testFlag").toBool())
    {
        _timer->start(_MS_2000_);
        _autoTestButton->setText(tr("停止自动检测"));
        emit newMessage(tr("开始转速表检测"),0);
        setProperty("testFlag", true);
    }
    else
    {
        _timer->stop();
        _autoTestButton->setText(tr("开始自动检测"));
        setProperty("testFlag",false);
    }
}

void CFunctionTestRotateTableWindow::timerUpdate()
{
    //***PLC
    if(_cur>=cf->_function_tpoint["meter_rspeed"].size())
        _cur=0;
    int val = cf->_function_tpoint["meter_rspeed"][_cur];

    if(val<=cf->_function_param["low_speed"])
        _signOutLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
    else
        _signOutLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");

    _currentValue->setValue(val);
    _statusLabel->setText(tr("请观察转速表，现在的值应该是 ") + QString::number(val) + tr(" rpm"));
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    c.can1_id[0] = 0x00;
    c.can1_id[1] = 0x04;
    c.can1_id[2] = 0xF0;
    c.can1_id[3] = 0x0C;
    QByteArray tmp = arrangeByte(val*8,2);
    c.can1_data[3] = tmp[0];
    c.can1_data[4] = tmp[1];
    c.can1_rate[0] = 0x1;
    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    _cur++;
}

void CFunctionTestRotateTableWindow::singleTestButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentValue->value();
        _statusLabel->setText(tr("请观察的转速表，现在的值应该是 ") + QString::number(val) + tr(" rpm"));
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        c.can1_id[0] = 0x00;
        c.can1_id[1] = 0x04;
        c.can1_id[2] = 0xF0;
        c.can1_id[3] = 0x0c;
        QByteArray tmp = arrangeByte(val*8,2);
        c.can1_data[3] = tmp[0];
        c.can1_data[4] = tmp[1];
        c.can1_rate[0] = 0x1;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}
