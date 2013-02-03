#include "capp.h"
#include "cfunctiontestvehiclevoltagewindow.h"

CFunctionTestVeHicleVoltageWindow::CFunctionTestVeHicleVoltageWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",false);

   //***UI CONPONENTS
    QVBoxLayout *top = new QVBoxLayout;

    QGroupBox *_mainGroupBox = new QGroupBox(tr("电压测试"));
    QGridLayout *_mainGrid = new QGridLayout(_mainGroupBox);
    _currentValue1 = new QComboBox;
    _currentValue2 = new QSpinBox;
    _currentValue2->setMinimum(0);
    _currentValue2->setMaximum(24);
    _autoTestButton = new QPushButton(tr("自动检测"));
    _singleTestButton1 = new QPushButton(tr("测单点"));
    _singleTestButton2 = new QPushButton(tr("测单点"));
    _statusLabel = new QLabel(tr("状态"));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    //***Layout
    _mainGrid->addWidget(_statusLabel,0,0,1,3);
    _mainGrid->addWidget(new QLabel(tr("整车电压：")),1,0);
    _mainGrid->addWidget(_currentValue1,1,1);
    _mainGrid->addWidget(new QLabel(tr("V")),1,2);
    _mainGrid->addWidget(_singleTestButton1,2,1);
    _mainGrid->addWidget(new QLabel(tr("排气制动电压：")),3,0);
    _mainGrid->addWidget(_currentValue2,3,1);
    _mainGrid->addWidget(new QLabel(tr("V")),3,2);
    _mainGrid->addWidget(_singleTestButton2,4,1);
    _mainGrid->addWidget(_autoTestButton,5,1);

    top->addWidget(_mainGroupBox);
    top->setSizeConstraint(QLayout::SetFixedSize);
    top->setAlignment(Qt::AlignCenter);
    setLayout(top);

    //***signals
    connect(_autoTestButton,SIGNAL(clicked()),this,SLOT(autoTestButtonClicked()));
    connect(_singleTestButton1,SIGNAL(clicked()),this,SLOT(singleTestButton1Clicked()));
    connect(_singleTestButton2,SIGNAL(clicked()),this,SLOT(singleTestButton2Clicked()));

    _timer = new QTimer;
    connect(_timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
    for(int i=0;i<cf->_function_tpoint["voltage"].size();i++)
        _currentValue1->addItem(QString::number(cf->_function_tpoint["voltage"][i]));
}

void CFunctionTestVeHicleVoltageWindow::autoTestButtonClicked()
{
    _cur=0;
    if(!property("testFlag").toBool())
    {
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        c._12v = 0x1;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
        _timer->start(_MS_2000_);
        _autoTestButton->setText(tr("停止自动检测"));
        emit newMessage(tr("开始电压检测"),0);
        setProperty("testFlag", true);
    }
    else
    {
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        c._12v = 0x0;
        c._xxv = 0x2;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        _timer->stop();
        _autoTestButton->setText(tr("开始自动检测"));
        setProperty("testFlag",false);
    }
}

void CFunctionTestVeHicleVoltageWindow::timerUpdate()
{
    int val;
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);

    //***PLC
    if(_cur>=(cf->_function_tpoint["voltage"].size() + cf->_function_tpoint["brake_voltage"].size()))
        _cur=0;

    if(_cur < cf->_function_tpoint["voltage"].size())
    {
        val = cf->_function_tpoint["voltage"][_cur];
        _currentValue1->setCurrentIndex(_cur);

        //***打开常电，切换到当前选择的电压
        c._12v = 0x1;
        if(val==12)
            c._xxv = 1;
        else if(val==24)
            c._xxv = 2;
        else if(val==36)
            c._xxv = 3;
        _statusLabel->setText(tr("请观察整车电压，现在的值应该是 ") + QString::number(val) + tr(" V"));
    }
    else
    {
        val = cf->_function_tpoint["brake_voltage"][_cur-cf->_function_tpoint["brake_voltage"].size()];
        _currentValue2->setValue(val);

        _statusLabel->setText(tr("请观察排气制动电压，现在的值应该是 ") + QString::number(val) + tr(" V"));
    }

    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

    _cur++;
}

void CFunctionTestVeHicleVoltageWindow::singleTestButton1Clicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentValue1->currentText().toInt();
        _statusLabel->setText(tr("请观察整车电压，现在的值应该是 ") + QString::number(val) + tr(" V"));
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        //***打开常电，切换到当前选择的电压
        c._12v = 0x1;
        if(val==12)
            c._xxv = 1;
        else if(val==24)
            c._xxv = 2;
        else if(val==36)
            c._xxv = 3;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}

void CFunctionTestVeHicleVoltageWindow::singleTestButton2Clicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentValue2->value();
        _statusLabel->setText(tr("请观察排气制动电压，现在的值应该是 ") + QString::number(val) + tr(" V"));
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        c.vout[cf->_map_voltage[0]] = val*10;
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}
