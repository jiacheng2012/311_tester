#include "cfunctiontestpressurewindow.h"
#include "capp.h"

CFunctionTestPressureWindow::CFunctionTestPressureWindow(QWidget *parent) :
    QWidget(parent)
{
    //***UI CONPONENTS
    QVBoxLayout *top = new QVBoxLayout;
    QGroupBox *_mainGroupBox = new QGroupBox(tr("气压测试"),this);
    QGridLayout *_mainGrid = new QGridLayout(_mainGroupBox);

    _currentParkingPressureValue = new QSpinBox;//驻车制动
    _currentParkingPressureValue->setMinimum(0);
    _currentParkingPressureValue->setMaximum(1000);
    _currentDrivingPressureValue = new QSpinBox;//行车制动
    _currentDrivingPressureValue->setMinimum(0);
    _currentDrivingPressureValue->setMaximum(1000);
    _currentBrakingPressure3Value = new QSpinBox;//行车制动
    _currentBrakingPressure3Value->setMinimum(0);
    _currentBrakingPressure3Value->setMaximum(1000);
    _currentBrakingPressure4Value = new QSpinBox;//行车制动
    _currentBrakingPressure4Value->setMinimum(0);
    _currentBrakingPressure4Value->setMaximum(1000);

    _autoTestButton = new QPushButton(tr("自动检测"));
    _singleTestButton1 = new QPushButton(tr("测单点"));
    _singleTestButton2 = new QPushButton(tr("测单点"));
    _singleTestButton3 = new QPushButton(tr("测单点"));
    _singleTestButton4 = new QPushButton(tr("测单点"));


        //气压低圆形 标识 label
    _pressureLowLabel = new QLabel;
    _pressureLowLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
    _pressureLowLabel->setFixedSize(30,30);
        //气压蜂鸣器输出
    _pressureBuzzerLabel = new QLabel;
    _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
    _pressureBuzzerLabel->setFixedSize(30,30);

    QLabel *_tempBitmapLabel = new QLabel;
    _tempBitmapLabel->setPixmap(QPixmap(":/res/气压低.bmp"));
    _tempBitmapLabel->setScaledContents(true);
    _tempBitmapLabel->setFixedSize(40,44);

    _statusLabel = new QLabel(tr("测试将检测所有四个气压值是否正确。"));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");

    //***Layout
    _mainGrid->addWidget(_statusLabel,0,0,1,3);
    _mainGrid->addWidget(new QLabel(tr("气压低报警：")),1,0);
    _mainGrid->addWidget(_tempBitmapLabel,1,1);
    _mainGrid->addWidget(_pressureLowLabel,1,2);

    _mainGrid->addWidget(new QLabel(tr("气压蜂鸣器输出：")),2,0);
    _mainGrid->addWidget(_pressureBuzzerLabel,2,1);

    _mainGrid->addWidget(new QLabel(tr("驻车制动压力(kpa)：")),3,0);
    _mainGrid->addWidget(_currentParkingPressureValue,3,1);
    _mainGrid->addWidget(_singleTestButton1,3,2);

    _mainGrid->addWidget(new QLabel(tr("行车制动压力(kpa)：")),5,0);
    _mainGrid->addWidget(_currentDrivingPressureValue,5,1);
    _mainGrid->addWidget(_singleTestButton2,5,2);

    _mainGrid->addWidget(new QLabel(tr("制动回路3压力(kpa)：")),7,0);
    _mainGrid->addWidget(_currentBrakingPressure3Value,7,1);
    _mainGrid->addWidget(_singleTestButton3,7,2);

    _mainGrid->addWidget(new QLabel(tr("制动回路4压力(kpa)：")), 9, 0);
    _mainGrid->addWidget(_currentBrakingPressure4Value, 9, 1);
    _mainGrid->addWidget(_singleTestButton4, 9, 2);

    _mainGrid->addWidget(_autoTestButton, 11, 1, 1, 2);

    top->addWidget(_mainGroupBox);
    top->setSizeConstraint(QLayout::SetFixedSize);
    top->setAlignment(Qt::AlignCenter);
    setLayout(top);

    //***signals
    connect(_autoTestButton,SIGNAL(clicked()),this,SLOT(autoTestButtonClicked()));
    connect(_singleTestButton1,SIGNAL(clicked()),this,SLOT(singleTestButton1Clicked()));
    connect(_singleTestButton2,SIGNAL(clicked()),this,SLOT(singleTestButton2Clicked()));
    connect(_singleTestButton3,SIGNAL(clicked()),this,SLOT(singleTestButton3Clicked()));
    connect(_singleTestButton4,SIGNAL(clicked()),this,SLOT(singleTestButton4Clicked()));

    _timer = new QTimer;
    connect(_timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFunctionTestPressureWindow::autoTestButtonClicked()
{
    _cur=0;
    if(!property("testFlag").toBool())
    {
        _timer->start(_MS_2000_);
        _autoTestButton->setText(tr("停止自动检测"));
        emit newMessage(tr("开始气压量检测"),0);
        setProperty("testFlag", true);
    }
    else
    {
        _timer->stop();
        _autoTestButton->setText(tr("开始自动检测"));
        setProperty("testFlag",false);
        _statusLabel = new QLabel(tr("测试将检测所有四个气压值是否正确。"));
    }
}

void CFunctionTestPressureWindow::timerUpdate()
{
    int val;

    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);

    //***PLC
    int num1 = cf->_function_tpoint["pressure_brake1"].size();
    int num2 = cf->_function_tpoint["pressure_brake2"].size();
    int num3 = cf->_function_tpoint["pressure_brake3"].size();
    int num4 = cf->_function_tpoint["pressure_brake4"].size();

    if(_cur>= ( num1+num2+num3+num4 ))
        _cur=0;

    if(_cur< num1)
    {
        val = cf->_function_tpoint["pressure_brake1"][_cur];
        if(val<=cf->_function_param["w_pressurel"])
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察驻车制动气压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯点亮！蜂鸣器鸣叫！"));
        }
        else
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察驻车制动气压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯熄灭！蜂鸣器停止！"));
        }
        _currentParkingPressureValue->setValue(val);

        //c.curout
        c.curout[cf->_map_current[1]] = getPressure(val*1000)*10;
    }
    else if(_cur<num1+num2)
    {
        val = cf->_function_tpoint["pressure_brake2"][_cur-num1];
        if(val<=cf->_function_param["w_pressurel"])
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察行车制动气压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯点亮！蜂鸣器鸣叫！"));
        }
        else
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察行车制动气压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯熄灭！蜂鸣器停止！"));
        }
        _currentDrivingPressureValue->setValue(val);
        //c.curout
        c.curout[cf->_map_current[2]] = getPressure(val*1000)*10;
    }
    else if(_cur<num1+num2+num3)
    {
        val = cf->_function_tpoint["pressure_brake3"][_cur-num1-num2];
        if(val<=cf->_function_param["w_pressurel"])
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察制动回路气压3，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯点亮！蜂鸣器鸣叫！"));
        }
        else
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察制动回路气压3，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯熄灭！蜂鸣器停止！"));
        }
        _currentBrakingPressure3Value->setValue(val);
        //c.curout
        c.curout[cf->_map_current[0]] = getPressure(val*1000)*10;
    }
    else
    {
        val = cf->_function_tpoint["pressure_brake4"][_cur-num1-num2-num3];
        if(val<=cf->_function_param["w_pressurel"])
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察制动回路气压4，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯点亮！蜂鸣器鸣叫！"));
        }
        else
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察制动回路气压4，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯熄灭！蜂鸣器停止！"));
        }
        _currentBrakingPressure4Value->setValue(val);
        //c.curout
        c.curout[cf->_map_current[3]] = getPressure(val*1000)*10;
    }

    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    _cur++;
}

void CFunctionTestPressureWindow::singleTestButton1Clicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentParkingPressureValue->value();
        if(val<=cf->_function_param["w_pressurel"])
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察驻车制动气压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯点亮！蜂鸣器鸣叫！"));
        }
        else
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察驻车制动气压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯熄灭！蜂鸣器停止！"));
        }
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        //c.curout
        c.curout[cf->_map_current[1]] = getPressure(val*1000)*10;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}

void CFunctionTestPressureWindow::singleTestButton2Clicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentDrivingPressureValue->value();
        if(val<=cf->_function_param["w_pressurel"])
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察行车制动气压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯点亮！蜂鸣器鸣叫！"));
        }
        else
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察行车制动气压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯熄灭！蜂鸣器停止！"));
        }
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        //c.curout
        c.curout[cf->_map_current[2]] = getPressure(val*1000)*10;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}

void CFunctionTestPressureWindow::singleTestButton3Clicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentBrakingPressure3Value->value();
        if(val<=cf->_function_param["w_pressurel"])
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察制动回路气压3，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯点亮！蜂鸣器鸣叫！"));
        }
        else
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察制动回路气压3，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯熄灭！蜂鸣器停止！"));
        }
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        //c.curout
        c.curout[cf->_map_current[0]] = getPressure(val*1000)*10;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}

void CFunctionTestPressureWindow::singleTestButton4Clicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentBrakingPressure4Value->value();
        if(val<=cf->_function_param["w_pressurel"])
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察制动回路气压4，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯点亮！蜂鸣器鸣叫！"));
        }
        else
        {
            _pressureBuzzerLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _pressureLowLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察制动回路气压4，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n气压低指示灯熄灭！蜂鸣器停止！"));
        }
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        //c.curout
        c.curout[cf->_map_current[3]] = getPressure(val*1000)*10;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}

