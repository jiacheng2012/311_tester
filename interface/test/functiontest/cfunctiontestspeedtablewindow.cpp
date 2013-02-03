#include "cfunctiontestspeedtablewindow.h"
#include "capp.h"

CFunctionTestSpeedTableWindow::CFunctionTestSpeedTableWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",false);

   //***UI CONPONENTS
    QVBoxLayout *top = new QVBoxLayout;

    QGroupBox *_mainGroupBox = new QGroupBox(tr("车速表测试"));
    QGridLayout *_mainGrid = new QGridLayout(_mainGroupBox);
    _currentValue = new QSpinBox;
    _currentValue->setMinimum(0);
    _currentValue->setMaximum(300);
    _autoTestButton = new QPushButton(tr("自动检测"));
    _singleTestButton = new QPushButton(tr("测单点"));
    _signOutLabel = new QLabel;
    _signOutLabel->setStyleSheet("border-radius:20px;background:#fff;border:1px solid #666;");
    _signOutLabel->setFixedSize(40,40);
    _statusLabel = new QLabel(tr("状态："));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");

    QLabel *_bitMap = new QLabel();
    _bitMap->setScaledContents(true);
    _bitMap->setPixmap(QPixmap(":/res/高低档切换.bmp"));
    _bitMap->setFixedSize(48,50);
    _bitMap->setProperty("picture",true);

    //***Layout
    _mainGrid->addWidget(_statusLabel,0,0,1,3);
    _mainGrid->addWidget(new QLabel(tr("高低档切换指示灯：")),1,0);
    _mainGrid->addWidget(_bitMap,1,1);
    _mainGrid->addWidget(_signOutLabel,1,2);
    _mainGrid->addWidget(new QLabel(tr("车速：")),2,0);
    _mainGrid->addWidget(_currentValue,2,1);
    _mainGrid->addWidget(new QLabel(tr("km/h")),2,2);
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

void CFunctionTestSpeedTableWindow::autoTestButtonClicked()
{
    _cur=0;
    if(!property("testFlag").toBool())
    {
        _timer->start(_MS_3000_);
        _autoTestButton->setText(tr("停止自动检测"));
        emit newMessage(tr("开始速度表检测"),0);
        setProperty("testFlag", true);
    }
    else
    {
        _timer->stop();
        _autoTestButton->setText(tr("开始自动检测"));
        setProperty("testFlag",false);
    }
}

void CFunctionTestSpeedTableWindow::timerUpdate()
{
    //***PLC
    if(_cur>=cf->_function_tpoint["meter_speed"].size())
        _cur=0;
    int val = cf->_function_tpoint["meter_speed"][_cur];
    if(val<=cf->_function_param["low_speed"])
    {
        _signOutLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
        _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
        _statusLabel->setText(tr("请观察速度表，现在的值应该是 ") + QString::number(val) + tr(" km/h，\n高低档切换指示灯点亮！"));
    }
    else
    {
        _signOutLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
        _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
        _statusLabel->setText(tr("请观察速度表，现在的值应该是 ") + QString::number(val) + tr(" km/h，\n高低档切换指示灯熄灭！"));
    }
    _currentValue->setValue(val);
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    QByteArray tb = arrangeByte( (double)val*((double)cf->_function_param["speed_pulse"]/3600.0),4);
    c.freqout[0] = tb[0];
    c.freqout[1] = tb[1];
    c.freqout[2] = tb[2];
    c.freqout[3] = tb[3];
    //***取力开关量置低位
    setMap(&c,2,0);
    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    _cur++;
}

void CFunctionTestSpeedTableWindow::singleTestButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        int val = _currentValue->value();
        if(val<=cf->_function_param["low_speed"])
            _signOutLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
        else
            _signOutLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");

        if(val<=cf->_function_param["low_speed"])
        {
            _signOutLabel->setStyleSheet("border-radius:15px;background:red;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察速度表，现在的值应该是 ") + QString::number(val) + tr(" km/h，\n高低档切换指示灯点亮！"));
        }
        else
        {
            _signOutLabel->setStyleSheet("border-radius:15px;background:#fff;border:1px solid #666;");
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:50px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("请观察速度表，现在的值应该是 ") + QString::number(val) + tr(" km/h，\n高低档切换指示灯熄灭！"));
        }

        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        QByteArray tb = arrangeByte( (double)val*((double)cf->_function_param["speed_pulse"]/3600.0),4);
        c.freqout[0] = tb[0];
        c.freqout[1] = tb[1];
        c.freqout[2] = tb[2];
        c.freqout[3] = tb[3];
        //***取力开关量置低位
        setMap(&c,2,0);
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else
    {
        emit newMessage(tr("请先停止自动检测！"),1);
    }
}

void CFunctionTestSpeedTableWindow::setMap(PLCCommandFrame_0x80* a,int id, int label)
{
        int j = cf->_map_ioin[id];
        int d = (int)(j/4);
        int r = j - d*4;

        a->ioo[d] &= ~(3<<(2*r));
        if(label==1)
            a->ioo[d] |= 2<<(2*r);
        else
            a->ioo[d] |= 1<<(2*r);

        //***差分IO
        int kk = ((CApp*)qApp)->_tjob->getIOInMapDifference(id);
        if(kk!=-1)
        {
            int n = (int)(kk/4);
            int o = kk - n*4;

            a->ioo[n] &= ~(3<<(2*o));
            a->ioo[n] |= 1<<(2*o);
        }
}
