#include "cfulltestanalogwindow.h"
#include "capp.h"

CFullTestAnalogWindow::CFullTestAnalogWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);
    setProperty("channel",0);
    setProperty("type",0);
    setProperty("id",0);
    setProperty("number",0);

    //***UI components
    QVBoxLayout *globalVLayout = new QVBoxLayout;

    _statusLabel_0 = new QLabel(tr("快捷键：通道选择 A(+) Q(-)，档位选择 W(+) S(-)"));
    _statusLabel_0->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");
    _channelSample = new QSpinBox();
    _prvButton = new QPushButton(tr("上一通道(A)"));
    _nxtButton = new QPushButton(tr("下一通道(Q)"));
    _analogType = new QComboBox();
    _analogType->addItem(tr("电流 4~20mA"));
    _analogType->addItem(tr("电阻 0~500欧"));
    _analogType->addItem(tr("电压 0~36伏"));
    _analogType->addItem(tr("功率电流 0~3000mA"));
    _analogType->setEnabled(false);
    _currentItem = new QLabel;
    _currentItem->setPixmap(QPixmap(":/res/flag.png"));
    _currentItem->setFixedSize(40,30);
    _currentItem->setScaledContents(true);
    _nameLabel = new QLabel(tr("(mA)"));
    _statusLabel = new QLabel(tr("点击按钮进行测试"));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    for(int i=0; i < _MAX_ATEST_POINT_; ++i)
    {
        _actualSampleValue[i] = new QSpinBox();
        _dashboardSampleValue[i] = new QLabel();
        _theorySampleValue[i]  = new QLabel();
        _startButton[i] = new QPushButton(tr("测试"));

        connect(_startButton[i],SIGNAL(clicked()),SLOT(startButtonClicked()));
        connect(_actualSampleValue[i],SIGNAL(valueChanged(int)),SLOT(actualValueChanged(int)));
    }
    //***Layout
    QVBoxLayout  *_mainVLayout = new QVBoxLayout;

    QGroupBox *_parmsetGroupBox = new QGroupBox(tr("测试参数设置"));
    QGridLayout *_parmsetHLayout = new QGridLayout(_parmsetGroupBox);
    _parmsetHLayout->addWidget(_statusLabel_0,0,0,1,4);
    _parmsetHLayout->addWidget(new QLabel(tr("采样信号通道：")),1,0);
    _parmsetHLayout->addWidget(_channelSample,1,1);
    _parmsetHLayout->addWidget(_prvButton,1,2);
    _parmsetHLayout->addWidget(_nxtButton,1,3);
    _parmsetHLayout->addWidget(new QLabel(tr("测试类型：")),1,4);
    _parmsetHLayout->addWidget(_analogType,1,5);

    QGroupBox *_resultGroupBox = new QGroupBox(tr("测试结果观察"));
    _resultGridLayout = new QGridLayout(_resultGroupBox);
    _resultGridLayout->addWidget(_statusLabel,0,0,1,2+_MAX_ATEST_POINT_);
    _resultGridLayout->addWidget(new QLabel(tr("模拟量实际值")),2,0);
    _resultGridLayout->addWidget(_nameLabel,2,1);
    _resultGridLayout->addWidget(new QLabel(tr("仪表板采样值")),3,0);
    _resultGridLayout->addWidget(new QLabel(tr("理论采样值")),4,0);
    for(int i=0; i < _MAX_ATEST_POINT_; ++i)
    {
        _resultGridLayout->addWidget(_currentItem,1,1);
        _resultGridLayout->addWidget(_actualSampleValue[i],2,i+2);
        _resultGridLayout->addWidget(_dashboardSampleValue[i],3,i+2);
        _dashboardSampleValue[i]->setEnabled(false);
        _resultGridLayout->addWidget(_theorySampleValue[i],4,i+2);
        _theorySampleValue[i]->setEnabled(false);
        _resultGridLayout->addWidget(_startButton[i],5,i+2);
    }
    _mainVLayout->addWidget(_parmsetGroupBox);

    globalVLayout->addLayout(_mainVLayout);
    globalVLayout->addWidget(_resultGroupBox, 0, Qt::AlignCenter);
    globalVLayout->setSizeConstraint(QLayout::SetFixedSize);
    globalVLayout->setContentsMargins(15,0,15,0);
    setLayout(globalVLayout);

    //***signal
    connect(_channelSample,SIGNAL(valueChanged(int)),SLOT(channelChanged(int)));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_analog_485(QByteArray)),this,SLOT(sendBackData485(QByteArray)));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_analog_232(QByteArray)),this,SLOT(sendBackData232(QByteArray)));
    connect(_prvButton,SIGNAL(clicked()),this,SLOT(prvButtonClicked()));
    connect(_nxtButton,SIGNAL(clicked()),this,SLOT(nxtButtonClicked()));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
    _channelSample->setMinimum(0);
    _channelSample->setMaximum( cf->_channel_type.size()-1 );

    channelChanged(0);
}

void CFullTestAnalogWindow::prvButtonClicked()
{
    int c = property("channel").toInt();

    c--;
    if(c<0)
        c = cf->_channel_type.size()-1;
    _channelSample->setValue(c);
    channelChanged(c);
}

void CFullTestAnalogWindow::nxtButtonClicked()
{
    int c =  property("channel").toInt();

    c++;
    if(c>=cf->_channel_type.size())
        c = 0;

    _channelSample->setValue(c);
    channelChanged(c);

}

void CFullTestAnalogWindow::channelChanged(int channel)
{
    setProperty("testFlag",0);
    int i;
    setProperty("channel",channel);
    setProperty("type", cf->_channel_type[channel]);
    setProperty("number",cf->_channel_value[channel].size());

    _analogType->setCurrentIndex(property("type").toInt());

    _resultGridLayout->removeWidget(_currentItem);
    _resultGridLayout->addWidget(_currentItem,1,2);

    for(i=0;i<cf->_channel_value[channel].size();i++)
    {
        _startButton[i]->setEnabled(true);
        _actualSampleValue[i]->setEnabled(true);
        _dashboardSampleValue[i]->setText("");
        _theorySampleValue[i]->setText("");

        _actualSampleValue[i]->setMaximum(1000000);
        _actualSampleValue[i]->setValue(cf->_channel_value[channel][i]);
        if(property("type").toInt()==ATYPE_CURRENT)
        {
            _theorySampleValue[i]->setText( QString::number(
                                                getTheorySample(cf->range_current_min,
                                                            cf->range_current_max,
                                                            cf->_channel_value[channel][i]*100,
                                                            4096)
                                                )
                                            );
            _actualSampleValue[i]->setMinimum(4);
            _actualSampleValue[i]->setMaximum(20);
            _nameLabel->setText(tr("mA"));
        }
        else if(property("type").toInt()==ATYPE_RESISTOR)
        {
            _theorySampleValue[i]->setText( QString::number(
                                                getTheorySample(cf->range_resistor_min,
                                                            cf->range_resistor_max,
                                                            cf->_channel_value[channel][i]*10,
                                                            4096)
                                                )
                                            );
            _actualSampleValue[i]->setMinimum(0);
            _actualSampleValue[i]->setMaximum(500);
            _nameLabel->setText(tr("Ω"));
        }
        else if(property("type").toInt()==ATYPE_VOLTAGE)
        {
            _theorySampleValue[i]->setText( QString::number(
                                                getTheorySample(cf->range_voltage_min,
                                                            cf->range_voltage_max,
                                                            cf->_channel_value[channel][i]*10,
                                                            4096)
                                                )
                                            );
            _actualSampleValue[i]->setMinimum(0);
            _actualSampleValue[i]->setMaximum(36);
            _nameLabel->setText(tr("V"));
        }
        else if(property("type").toInt()==ATYPE_POWER)
        {
            _theorySampleValue[i]->setText( QString::number(
                                                getTheorySample(cf->range_power_min,
                                                            cf->range_power_max,
                                                            cf->_channel_value[channel][i]/10,
                                                            4096)
                                                )
                                            );
            _actualSampleValue[i]->setMinimum(0);
            _actualSampleValue[i]->setMaximum(3000);
            _nameLabel->setText(tr("mA"));
        }
    }

    for(i=cf->_channel_value[channel].size();i<_MAX_ATEST_POINT_;i++)
    {
        _startButton[i]->setEnabled(false);
        _actualSampleValue[i]->setEnabled(false);
        _theorySampleValue[i]->setText("");
        _dashboardSampleValue[i]->setText("");
    }

    setProperty("id",0);
    _resultGridLayout->removeWidget(_currentItem);
    _resultGridLayout->addWidget(_currentItem,1,property("id").toInt()+2);
}

void CFullTestAnalogWindow::startButtonClicked()
{
    QPushButton* sc=qobject_cast<QPushButton*>(sender());

    for(int i=0;i<_MAX_ATEST_POINT_;i++)
    {
        if(sc ==_startButton[i])
        {
            setProperty("id",i);
            break;
        }
    }
    startTest();
}

void CFullTestAnalogWindow::actualValueChanged(int index)
{
    QSpinBox* sc=qobject_cast<QSpinBox*>(sender());

    for(int i=0;i<_MAX_ATEST_POINT_;i++)
    {
        if(sc ==_actualSampleValue[i])
        {
            setProperty("id",i);
            break;
        }
    }

    startTest();
}


void CFullTestAnalogWindow::startTest()
{
    setProperty("testFlag",1);

    if(property("type").toInt()==ATYPE_POWER)
    {
        //***仪表打开功率负载开关
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[2],findChannelIndex(property("type").toInt(),property("channel").toInt()));

        QByteArray tmp1=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp1);
    }
    else
    {
        //***仪表关闭功率负载开关
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        b.command[2]=0;

        QByteArray tmp1=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp1);
    }

    //***PLC
    int val = _actualSampleValue[property("id").toInt()]->value();
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    setMap(&c,val);

    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

    _resultGridLayout->removeWidget(_currentItem);
    _resultGridLayout->addWidget(_currentItem,1,property("id").toInt()+2);
}

void CFullTestAnalogWindow::sendBackData485(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        PLCCResponseFrame_0x81 rt = frame81FromData(a);
    }
}

void CFullTestAnalogWindow::sendBackData232(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        double val;
        hardwareTestRespondFrame_0x20 rt = frame20FromData(a);

        _dashboardSampleValue[property("id").toInt()]->setText( QString::number(rt.ao[property("channel").toInt()]));

        double s = _theorySampleValue[property("id").toInt()]->text().toInt();
        double b = rt.ao[property("channel").toInt()];
        double e = b*SAMPLE_ERROR;
        if( fabs( s - b) > e )
        {
            _statusLabel->setStyleSheet("font:bold 16px;color:red;max-height:26px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("当前测试通道为：") + QString::number(property("channel").toInt()) + tr(" ，理论采样值为：") +
                                  QString::number(_theorySampleValue[property("id").toInt()]->text().toInt()) + tr(" ，实际采样值为：") +
                                  QString::number(_dashboardSampleValue[property("id").toInt()]->text().toInt()) + tr(" ，测试不正常。"));
        }
        else
        {
            _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");
            _statusLabel->setText(tr("当前测试通道为：") + QString::number(property("channel").toInt()) + tr(" ，理论采样值为：") +
                                  QString::number(_theorySampleValue[property("id").toInt()]->text().toInt()) + tr(" ，实际采样值为：") +
                                  QString::number(_dashboardSampleValue[property("id").toInt()]->text().toInt()) + tr(" ，测试正常。"));
        }
    }
}

void CFullTestAnalogWindow::setMap(PLCCommandFrame_0x80* a,int val)
{
    if(property("type").toInt()==ATYPE_CURRENT)
    {
        a->_xxv = 0x2;
        a->_12v = 0x1;
        int k = findChannelIndex(property("type").toInt(),property("channel").toInt());
        int j = cf->_map_current[k];
        a->curout[j] = val*10;
    }
    else if(property("type").toInt()==ATYPE_RESISTOR)
    {
        a->_xxv = 0x2;
        a->_12v = 0x1;
        int k = findChannelIndex(property("type").toInt(),property("channel").toInt());
        int j = cf->_map_resistor[k];
        if(calResistor(val)==0xFF)
            newMessage(tr("所选阻值硬件无法实现，请重新选择！"),1);
        else
            a->resout[j] = calResistor(val);
    }
    else if(property("type").toInt()==ATYPE_VOLTAGE)
    {
    //***特殊仪表内部采样电压通道
        if( findMeterVoltageSampleChannel(property("type").toInt(),property("channel").toInt()) )
        {
            a->_12v = 0x1;
            if(val==12)
                a->_xxv = 0x1;
            else if(val==24)
                a->_xxv = 0x2;
            else if(val==36)
                a->_xxv = 0x3;
            else
            {
                newMessage(tr("仪表电源电压值只能是+12，+24或者+36V！请重新设置值。"),1);
                a->_xxv = 0x2;
            }
        }
        else
        {
            int k = findChannelIndex(property("type").toInt(),property("channel").toInt());
            int j = cf->_map_voltage[k];
            QByteArray tmp = arrangeByte(val*10,2);
            a->vout[j*2] = tmp[0];
            a->vout[j*2+1] = tmp[1];
        }
    }
    else if(property("type").toInt()==ATYPE_POWER)
    {
        a->_12v = 0x1;
        a->_xxv = 0x2;
        int k = findChannelIndex(property("type").toInt(),property("channel").toInt());
        int j = cf->_map_power[k];

        QByteArray b = setupLoader(val,property("type").toInt(),k);
        a->ioi[0] = b[0];
        a->ioi[1] = b[1];
    }
}

int CFullTestAnalogWindow::findChannelIndex(int t,int c)
{
    int index=0;

    for(int i=0;i<cf->_channel_type.size();i++)
    {
        if(t==cf->_channel_type[i]&&i<c)
            index++;
    }

    return index;
}

bool CFullTestAnalogWindow::findMeterVoltageSampleChannel(int type,int channel)
{
    int c = findChannelIndex(type,channel);
    return (cf->_map_meter_voltage == cf->_map_voltage[c]);
}

QByteArray CFullTestAnalogWindow::setupLoader(int val,int type,int channel)
{
    QByteArray a;
    quint8  a0=0,a1=0;

    if(channel<2)
    {
        setBit(a0,1);
        if(channel==1)
            setBit(a0,0);

        if(val==200)    // 120
        {
            setBit(a0,4);
        }
        else if(val==890) // 27
        {
            setBit(a0,5);
        }
        else if(val==1780) //27|27
        {
            setBit(a0,5);
            setBit(a0,6);
        }
        else if(val==1090) //120|27
        {
            setBit(a0,4);
            setBit(a0,5);
        }
        else if(val==1980) //120|27|27
        {
            setBit(a0,4);
            setBit(a0,5);
            setBit(a0,6);
        }
        else if(val==0)
        {
            a0=0x0;
            a1=0x0;
        }
        else
        {
            a0=0x0;
            a1=0x0;
            newMessage(tr("功率负载电流值只能是0mA，200mA，889mA，1089mA，1778mA或者1979mA！请重新设置值。"),1);
        }
    }
    else
    {
        setBit(a0,3);
        if(channel==3)
            setBit(a0,2);

        if(val==200)    // 120
        {
            setBit(a0,7);
        }
        else if(val==890) // 27
        {
            setBit(a1,0);
        }
        else if(val==1780) //27|27
        {
            setBit(a1,0);
            setBit(a1,1);
        }
        else if(val==1090) //120|27
        {
            setBit(a0,7);
            setBit(a1,0);
        }
        else if(val==1980) //120|27|27
        {
            setBit(a0,7);
            setBit(a1,0);
            setBit(a1,1);
        }
        else if(val==0)
        {
            a0=0x0;
            a1=0x0;
        }
        else
        {
            a0=0x0;
            a1=0x0;
            newMessage(tr("功率负载电流值只能是0mA，200mA，889mA，1089mA，1778mA或者1979mA！请重新设置值。"),1);
        }
    }
    a[0] = a0;
    a[1] = a1;

    return a;
}

/*快捷键*/
void CFullTestAnalogWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {

        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_S)
        {
            setProperty("testFlag",1);
            if(property("id").toInt() == 0)
            {
                setProperty("id", property("number").toInt() - 1);
            }
            else
                setProperty("id", property("id").toInt() - 1);

            _actualSampleValue[property("id").toInt()]->setFocus();
            startTest();
        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_W)
        {
            setProperty("testFlag",1);
            if(property("id").toInt() == property("number").toInt() - 1)
            {
                setProperty("id", 0);
            }
            else
                setProperty("id", property("id").toInt() + 1);
            _actualSampleValue[property("id").toInt()]->setFocus();
            startTest();
        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_A)
        {
            prvButtonClicked();
        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Q)
        {
            nxtButtonClicked();
        }
    }
}
