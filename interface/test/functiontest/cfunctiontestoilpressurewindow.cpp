#include "cfunctiontestoilpressurewindow.h"
#include "capp.h"

CFunctionTestOilPressureWindow::CFunctionTestOilPressureWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",false);

   //***UI CONPONENTS
    QVBoxLayout *top = new QVBoxLayout;

    QGroupBox *_mainGroupBox = new QGroupBox(tr("机油压力测试"));
    QGridLayout *_mainGrid = new QGridLayout(_mainGroupBox);
    _currentValue = new QSpinBox;
    _currentValue->setMinimum(0);
    _currentValue->setMaximum(1200);
    _autoTestButton = new QPushButton(tr("自动检测"));
    _singleTestButton = new QPushButton(tr("测单点"));
    _signOutLabel = new QLabel;
    _signOutLabel->setStyleSheet("border-radius:20px;background:#fff;border:1px solid #666;");
    _signOutLabel->setFixedSize(40,40);
    _statusLabel = new QLabel(tr("状态"));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");

    QLabel *_bitMap = new QLabel();
    _bitMap->setScaledContents(true);
    _bitMap->setPixmap(QPixmap(":/res/油压低.bmp"));
    _bitMap->setFixedSize(48,50);
    _bitMap->setProperty("picture",true);


    //***Layout
    _mainGrid->addWidget(_statusLabel,0,0,1,3);
    _mainGrid->addWidget(new QLabel(tr("油压低报警")),1,0);
    _mainGrid->addWidget(_bitMap,1,1);
    _mainGrid->addWidget(_signOutLabel,1,2);
    _mainGrid->addWidget(new QLabel(tr("机油电压：")),2,0);
    _mainGrid->addWidget(_currentValue,2,1);
    _mainGrid->addWidget(new QLabel(tr("kpa")),2,2);
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

void CFunctionTestOilPressureWindow::autoTestButtonClicked()
{
    _cur=0;
    if(!property("testFlag").toBool())
    {
        _timer->start(_MS_2000_);
        _autoTestButton->setText(tr("停止自动检测"));
        emit newMessage(tr("开始机油油压检测"),0);
        setProperty("testFlag", true);
    }
    else
    {
        _timer->stop();
        _autoTestButton->setText(tr("开始自动检测"));
        setProperty("testFlag",false);
    }
}

void CFunctionTestOilPressureWindow::timerUpdate()
{
    //***PLC
    if(_cur>=cf->_function_tpoint["pressure_oil"].size())
        _cur=0;
    int val = cf->_function_tpoint["pressure_oil"][_cur];
    if(val<=cf->_function_param["w_lpressure"])
    {
        _signOutLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
        _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
        _statusLabel->setText(tr("请观察机油油压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n机油压力指示灯点亮！"));
    }
    else
    {
        _signOutLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
        _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
        _statusLabel->setText(tr("请观察机油油压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n机油压力指示灯熄灭！"));
    }

    _currentValue->setValue(val);
    PLCCommandFrame_0x80 c={0};
    //c.curout
    c.curout[cf->_map_current[1]] = getPressure(val*1000)*10;
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    c.can1_id[0] = 0x00;
    c.can1_id[1] = 0xEF;
    c.can1_id[2] = 0xFE;
    c.can1_id[3] = 0x18;
    c.can1_data[3] = val/4;
    c.can1_rate[0] = 0x10;
    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    _cur++;
}

void CFunctionTestOilPressureWindow::singleTestButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentValue->value();
        if(val<=cf->_function_param["w_lpressure"])
        {
            _signOutLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察机油油压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n机油压力指示灯点亮！"));
        }
        else
        {
            _signOutLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察机油油压，现在的值应该是 ") + QString::number(val) + tr(" kpa，\n机油压力指示灯熄灭！"));
        }
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        c.can1_id[0] = 0x00;
        c.can1_id[1] = 0xEF;
        c.can1_id[2] = 0xFE;
        c.can1_id[3] = 0x18;
        c.can1_data[3] = val/4;
        c.can1_rate[0] = 0x10;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}
