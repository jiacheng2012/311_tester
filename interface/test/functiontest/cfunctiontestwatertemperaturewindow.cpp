#include "cfunctiontestwatertemperaturewindow.h"
#include "capp.h"

CFunctionTestWaterTemperatureWindow::CFunctionTestWaterTemperatureWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",false);

    //***UI CONPONENTS
    QVBoxLayout *top = new QVBoxLayout;
    QGroupBox *_mainGroupBox = new QGroupBox(tr("水温表测试"),this);
    QGridLayout *_mainGrid = new QGridLayout(_mainGroupBox);

    _currentValue = new QSpinBox;
    _currentValue->setMinimum(-40);
    _currentValue->setMaximum(210);
    _autoTestButton = new QPushButton(tr("自动检测"));
    _singleTestButton = new QPushButton(tr("测单点"));

        //水温高
    _waterTemperatureHighLabel = new QLabel;
    _waterTemperatureHighLabel->setStyleSheet("border-radius:20px;background:#fff;border:1px solid #666;");
    _waterTemperatureHighLabel->setFixedSize(40,40);
        //蜂鸣器
    _waterBuzzerLabel = new QLabel;
    _waterBuzzerLabel->setStyleSheet("border-radius:20px;background:#fff;border:1px solid #666;");
    _waterBuzzerLabel->setFixedSize(40,40);

    QLabel *_tempBitmapLabel = new QLabel;
    _tempBitmapLabel->setPixmap(QPixmap(":/res/水温高.bmp"));
    _tempBitmapLabel->setScaledContents(true);
    _tempBitmapLabel->setFixedSize(40,44);
    _statusLabel = new QLabel;
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");

    //***Layout
    _mainGrid->addWidget(_statusLabel,0,0,1,3);
    _mainGrid->addWidget(new QLabel(tr("水温高报警：")),1,0);
    _mainGrid->addWidget(_tempBitmapLabel,1,1);
    _mainGrid->addWidget(_waterTemperatureHighLabel,1,2);

    _mainGrid->addWidget(new QLabel(tr("水温蜂鸣器输出：")),2,0);
    _mainGrid->addWidget(_waterBuzzerLabel,2,1);

    _mainGrid->addWidget(new QLabel(tr("水温：")),3,0);
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

void CFunctionTestWaterTemperatureWindow::autoTestButtonClicked()
{
    _cur=0;
    if(!property("testFlag").toBool())
    {
        _timer->start(_MS_2000_);
        _autoTestButton->setText(tr("停止自动检测"));
        emit newMessage(tr("开始水温表检测"),0);
        setProperty("testFlag", true);
    }
    else
    {
        _timer->stop();
        _autoTestButton->setText(tr("开始自动检测"));
        setProperty("testFlag",false);
    }
}

void CFunctionTestWaterTemperatureWindow::timerUpdate()
{
    //***PLC
    if(_cur>=cf->_function_tpoint["meter_wtemp"].size())
        _cur=0;
    int val = cf->_function_tpoint["meter_wtemp"][_cur];

    if(val>=cf->_function_param["w_cwater"])
    {
        _waterBuzzerLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
        _waterTemperatureHighLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
        _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
        _statusLabel->setText(tr("请观察水温表，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n水温高指示灯点亮！蜂鸣器鸣叫！"));
    }
    else
    {
        _waterBuzzerLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
        _waterTemperatureHighLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
        _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
        _statusLabel->setText(tr("请观察水温表，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n水温高指示灯熄灭！蜂鸣器停止！"));
    }

    _currentValue->setValue(val);
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    c.can1_id[0] = 0x00;
    c.can1_id[1] = 0xEE;
    c.can1_id[2] = 0xFE;
    c.can1_id[3] = 0x18;
    c.can1_data[0] = val+40;
    c.can1_rate[0] = 0x10;
    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    _cur++;
}

void CFunctionTestWaterTemperatureWindow::singleTestButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentValue->value();
        if(val>=cf->_function_param["w_cwater"])
        {
            _waterBuzzerLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _waterTemperatureHighLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察水温表，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n水温高指示灯点亮！蜂鸣器鸣叫！"));
        }
        else
        {
            _waterBuzzerLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _waterTemperatureHighLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察水温表，现在的值应该是 ") + QString::number(val) + tr(" ℃，\n水温高指示灯熄灭！蜂鸣器停止！"));
        }

        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        c.can1_id[0] = 0x00;
        c.can1_id[1] = 0xEE;
        c.can1_id[2] = 0xFE;
        c.can1_id[3] = 0x18;
        c.can1_data[0] = val+40;
        c.can1_rate[0] = 0x10;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}

