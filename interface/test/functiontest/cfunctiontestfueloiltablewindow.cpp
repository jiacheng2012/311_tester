#include "cfunctiontestfueloiltablewindow.h"
#include "capp.h"

CFunctionTestFuelOilTableWindow::CFunctionTestFuelOilTableWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",false);

   //***UI CONPONENTS
    QVBoxLayout *top = new QVBoxLayout;

    QGroupBox *_mainGroupBox = new QGroupBox(tr("燃油表测试"));
    QGridLayout *_mainGrid = new QGridLayout(_mainGroupBox);
    _currentValue = new QSpinBox;
    _currentValue->setMinimum(0);
    _currentValue->setMaximum(100);
    _autoTestButton = new QPushButton(tr("自动检测"));
    _singleTestButton = new QPushButton(tr("测单点"));
    _signOutLabel = new QLabel;
    _signOutLabel->setStyleSheet("border-radius:20px;background:#fff;border:1px solid #666;");
    _signOutLabel->setFixedSize(40,40);
    _statusLabel = new QLabel(tr("状态"));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");

    QLabel *_bitMap = new QLabel();
    _bitMap->setScaledContents(true);
    _bitMap->setPixmap(QPixmap(":/res/燃油报警.bmp"));
    _bitMap->setFixedSize(48,50);
    _bitMap->setProperty("picture",true);

    //***Layout
    _mainGrid->addWidget(_statusLabel,0,0,1,3);
    _mainGrid->addWidget(new QLabel(tr("燃油报警")),1,0);
    _mainGrid->addWidget(_bitMap,1,1);
    _mainGrid->addWidget(_signOutLabel,1,2);
    _mainGrid->addWidget(new QLabel(tr("燃油：")),2,0);
    _mainGrid->addWidget(_currentValue,2,1);
    _mainGrid->addWidget(new QLabel(tr("%")),2,2);
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

void CFunctionTestFuelOilTableWindow::autoTestButtonClicked()
{
    _cur=0;
    if(!property("testFlag").toBool())
    {
        _timer->start(_MS_2000_);
        _autoTestButton->setText(tr("停止自动检测"));
        emit newMessage(tr("开始燃油表检测"),0);
        setProperty("testFlag", true);
    }
    else
    {
        _timer->stop();
        _autoTestButton->setText(tr("开始自动检测"));
        setProperty("testFlag",false);
    }
}

void CFunctionTestFuelOilTableWindow::timerUpdate()
{
 //***PLC
    if(_cur>=cf->_function_tpoint["meter_oil"].size())
        _cur=0;
    int val = cf->_function_tpoint["meter_oil"][_cur];

    if(val<=cf->_function_param["w_opressure"])
    {
        _signOutLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
        _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
        _statusLabel->setText(tr("请观察燃油表，现在的值应该是 ") + QString::number(val) + tr(" %，\n燃油低指示灯点亮！"));
    }
    else
    {
        _signOutLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
        _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
        _statusLabel->setText(tr("请观察燃油表，现在的值应该是 ") + QString::number(val) + tr(" %，\n燃油低指示灯熄灭！"));
    }

    _currentValue->setValue(val);
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    c.resout[cf->_map_resistor[0]] = getOilResistor(val);
    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    _cur++;
}

void CFunctionTestFuelOilTableWindow::singleTestButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentValue->value();
        if(val<=cf->_function_param["w_opressure"])
        {
            _signOutLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察燃油表，现在的值应该是 ") + QString::number(val) + tr(" %，\n燃油低指示灯点亮！"));
        }
        else
        {
            _signOutLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察燃油表，现在的值应该是 ") + QString::number(val) + tr(" %，\n燃油低指示灯熄灭！"));
        }
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        c.resout[cf->_map_resistor[0]] = getOilResistor(val);
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}
