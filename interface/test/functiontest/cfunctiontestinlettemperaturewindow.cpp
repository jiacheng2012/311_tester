#include "cfunctiontestinlettemperaturewindow.h"
#include "capp.h"

CFunctionTestInletTemperatureWindow::CFunctionTestInletTemperatureWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",false);

    //***UI CONPONENTS
    QVBoxLayout *top = new QVBoxLayout;
    QGroupBox *_mainGroupBox = new QGroupBox(tr("进气温度测试"),this);
    QGridLayout *_mainGrid = new QGridLayout(_mainGroupBox);

    _currentValue = new QSpinBox;
    _currentValue->setMinimum(-40);
    _currentValue->setMaximum(210);
    _autoTestButton = new QPushButton(tr("自动检测"));
    _singleTestButton = new QPushButton(tr("测单点"));

    _fanHighSpeedLabel = new QLabel;
    _fanHighSpeedLabel->setStyleSheet("border-radius:20px;background:#fff;border:1px solid #666;");
    _fanHighSpeedLabel->setFixedSize(40,40);
    _fanLowSpeedLabel = new QLabel;
    _fanLowSpeedLabel->setStyleSheet("border-radius:20px;background:#fff;border:1px solid #666;");
    _fanLowSpeedLabel->setFixedSize(40,40);
    _statusLabel = new QLabel(tr("状态"));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");


    //***Layout
    _mainGrid->addWidget(_statusLabel,0,0,1,3);
    _mainGrid->addWidget(new QLabel(tr("风扇高速输出：")),1,0);
    _mainGrid->addWidget(_fanHighSpeedLabel,1,1);

    _mainGrid->addWidget(new QLabel(tr("风扇低速输出：")),2,0);
    _mainGrid->addWidget(_fanLowSpeedLabel,2,1);

    _mainGrid->addWidget(new QLabel(tr("进气温度：")),3,0);
    _mainGrid->addWidget(_currentValue,3,1);
    _mainGrid->addWidget(new QLabel(tr("℃")),3,2);

    _mainGrid->addWidget(_autoTestButton,4,1);
    _mainGrid->addWidget(_singleTestButton,4,2);

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

void CFunctionTestInletTemperatureWindow::autoTestButtonClicked()
{
    _cur=0;
    if(!property("testFlag").toBool())
    {
        _timer->start(_MS_2000_);
        _autoTestButton->setText(tr("停止自动检测"));
        emit newMessage(tr("开始进气温度检测"),0);
        setProperty("testFlag", true);
    }
    else
    {
        _timer->stop();
        _autoTestButton->setText(tr("开始自动检测"));
        setProperty("testFlag",false);
    }
}

void CFunctionTestInletTemperatureWindow::timerUpdate()
{
    //***PLC
    _cur++;
    if(_cur>=cf->_function_tpoint["intake_wtemp"].size())
        _cur=0;
    int val = cf->_function_tpoint["intake_wtemp"][_cur];

    if(val>=cf->_function_param["fan_lspeed_temp"])
    {
        _fanLowSpeedLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
        _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
        if(val>=cf->_function_param["fan_hspeed_temp"])
        {
            _fanHighSpeedLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setText(tr("请观察进气温度，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n高速风扇输出，低速风扇输出！"));
        }
        else
        {
            _fanHighSpeedLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setText(tr("请观察进气温度，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n高速风扇停止，低速风扇输出！"));
        }
    }
    else
    {
        _fanLowSpeedLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
        if(val>=cf->_function_param["fan_hspeed_temp"])
        {
            _fanHighSpeedLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察进气温度，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n高速风扇输出，低速风扇停止！"));
        }
        else
        {
            _fanHighSpeedLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#fff;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察进气温度，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n高速风扇停止，低速风扇停止！"));
        }
    }

    _currentValue->setValue(val);
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    c.can1_id[0] = 0x00;
    c.can1_id[1] = 0xF6;
    c.can1_id[2] = 0xFE;
    c.can1_id[3] = 0x18;
    c.can1_data[1] = val+40;
    c.can1_rate[0] = 0x10;
    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
}

void CFunctionTestInletTemperatureWindow::singleTestButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentValue->value();
        if(val>=cf->_function_param["fan_lspeed_temp"])
        {
            _fanLowSpeedLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            if(val>=cf->_function_param["fan_hspeed_temp"])
            {
                _fanHighSpeedLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
                _statusLabel->setText(tr("请观察进气温度，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n高速风扇输出，低速风扇输出！"));
            }
            else
            {
                _fanHighSpeedLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
                _statusLabel->setText(tr("请观察进气温度，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n高速风扇停止，低速风扇输出！"));
            }
        }
        else
        {
            _fanLowSpeedLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            if(val>=cf->_function_param["fan_hspeed_temp"])
            {
                _fanHighSpeedLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
                _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
                _statusLabel->setText(tr("请观察进气温度，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n高速风扇输出，低速风扇停止！"));
            }
            else
            {
                _fanHighSpeedLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
                _statusLabel->setStyleSheet("font:bold 16px;color:#fff;max-height:50px;min-height:26px;background:#CCFF99;");
                _statusLabel->setText(tr("请观察进气温度，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n高速风扇停止，低速风扇停止！"));
            }
        }
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        c.can1_id[0] = 0x00;
        c.can1_id[1] = 0xF6;
        c.can1_id[2] = 0xFE;
        c.can1_id[3] = 0x18;
        c.can1_data[1] = val+40;
        c.can1_rate[0] = 0x10;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}
