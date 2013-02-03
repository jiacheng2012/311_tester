#include "CAdjustTestAnalogWindow.h"
#include "capp.h"

CAdjustTestAnalogWindow::CAdjustTestAnalogWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);
    setProperty("type",0);
    setProperty("id",0);
    setProperty("channel",0);
    setProperty("read",-1);
    setProperty("number",0);

    //***UI components
    QVBoxLayout *topLevelLayout = new QVBoxLayout;

    _statusLabel_0 = new QLabel(tr("选择模拟量测试通道。快捷键为："));
    _statusLabel_0->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");
    _channelSample = new QSpinBox();
    _analogType = new QComboBox();
    _prvButton = new QPushButton(tr("上一通道"));
    _nxtButton = new QPushButton(tr("下一通道"));

    _analogType->addItem(tr("电流 4~20mA"));
    _analogType->addItem(tr("电阻 0~500欧"));
    _analogType->addItem(tr("电压 0~36伏"));
    _analogType->addItem(tr("功率电流 0~3000mA"));
    _analogType->setEnabled(false);
    _currentItem = new QLabel;
    _currentItem->setPixmap(QPixmap(":/res/flag.png"));
    _currentItem->setFixedSize(40,30);
    _currentItem->setScaledContents(true);
    _statusLabel = new QLabel(tr("测试的时候每一项都观察几秒钟等采样值稳定。快捷键为："));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");
    _stopButton = new QPushButton(tr("停止所有测试"));

    _savaStatusLabel = new QLabel;
    _savaStatusLabel->setStyleSheet("background:wheat;");
    _savaAllAdjustButton = new QPushButton(tr("保存该模拟通道调校参数\n(CTRL + S)"));//保存所有调校
    _readButton = new QPushButton(tr("读取该模拟通道调校参数\n(R)"));
    _writeButton = new QPushButton(tr("参数固化写入仪表"));

    for(int i=0; i < _MAX_AADJUST_POINT_; ++i)
    {
        _actualValue[i] = new QSpinBox();
        _theorySampleValue[i]  = new QLabel();
        _sampleValue[i] = new QLabel();
        _savedSampleValue[i] = new QLabel();
        _startButton[i] = new QPushButton(tr("测试"));

        connect(_startButton[i],SIGNAL(clicked()),SLOT(startButtonClicked()));
        connect(_actualValue[i],SIGNAL(valueChanged(int)),SLOT(actualValueChanged(int)));
    }
    //***Layout
    QGroupBox *_parmsetGroupBox = new QGroupBox(tr("测试参数设置"));
    QGridLayout *_parmsetGrid = new QGridLayout(_parmsetGroupBox);
    _parmsetGrid->addWidget(_statusLabel_0,0,0,1,4);
    _parmsetGrid->addWidget(new QLabel(tr("采样信号通道：")),1,0);
    _parmsetGrid->addWidget(_channelSample,1,1);
    _parmsetGrid->addWidget(_prvButton,1,2);
    _parmsetGrid->addWidget(_nxtButton,1,3);
    _parmsetGrid->addWidget(new QLabel(tr("测试类型：")),2,0);
    _parmsetGrid->addWidget(_analogType,2,1);

    QGroupBox *_resultGroupBox = new QGroupBox(tr("测试结果观察"));
    _resultGridLayout = new QGridLayout(_resultGroupBox);
    _resultGridLayout->addWidget(_statusLabel,0,0,1,1+_MAX_AADJUST_POINT_);
    _resultGridLayout->addWidget(new QLabel(tr("模拟量实际值(mA)")),2,0);
    _resultGridLayout->addWidget(new QLabel(tr("理论采样值")),3,0);
    _resultGridLayout->addWidget(new QLabel(tr("仪表板当前采样值")),4,0);
    _resultGridLayout->addWidget(new QLabel(tr("仪表板记录采样值")),5,0);
    _resultGridLayout->addWidget(new QLabel(tr("调校操作")),6,0);
    _resultGridLayout->addWidget(new QLabel(tr("存储操作")),8,0);

    for(int i=0; i < _MAX_AADJUST_POINT_; ++i)
    {
        _resultGridLayout->addWidget(_currentItem,1,1);
        _resultGridLayout->addWidget(_actualValue[i],2,i+1);
        _resultGridLayout->addWidget(_theorySampleValue[i],3,i+1);
        _resultGridLayout->addWidget(_sampleValue[i],4,i+1);
        _resultGridLayout->addWidget(_savedSampleValue[i],5,i+1);
        _resultGridLayout->addWidget(_startButton[i],6,i+1);
    }
    _resultGridLayout->addWidget(_savaStatusLabel,7,1,1,4);
    _resultGridLayout->addWidget(_readButton,8,1,1,2);
    _resultGridLayout->addWidget(_savaAllAdjustButton,8,3,1,2);
    _resultGridLayout->addWidget(_writeButton,9,1,1,2);
    _resultGridLayout->addWidget(_stopButton,9,3,1,2);
    _resultGridLayout->setSizeConstraint(QLayout::SetFixedSize);

    topLevelLayout->addWidget(_parmsetGroupBox);
    topLevelLayout->addWidget(_resultGroupBox);
    topLevelLayout->setContentsMargins(0,0,0,0);
    topLevelLayout->setSpacing(5);
    topLevelLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(topLevelLayout);
    _stopButton->setEnabled(false);

    //***signal
    connect(_channelSample,SIGNAL(valueChanged(int)),SLOT(channelChanged(int)));
    connect((CApp*)qApp,SIGNAL(sendBackAdjustTestData_analog_485(QByteArray)),this,SLOT(sendBackData485(QByteArray)));
    connect((CApp*)qApp,SIGNAL(sendBackAdjustTestData_analog_232(CDataFrame)),this,SLOT(sendBackData232(CDataFrame)));
    connect(_stopButton,SIGNAL(clicked()),this,SLOT(stopButtonClicked()));

    connect(_writeButton,SIGNAL(clicked()),this,SLOT(writeButtonClicked()));
    connect(_readButton,SIGNAL(clicked()),this,SLOT(readButtonClicked()));
    connect(_savaAllAdjustButton,SIGNAL(clicked()),this,SLOT(saveAllAdjustButtonClicked()));

    connect(_prvButton,SIGNAL(clicked()),this,SLOT(prvButtonClicked()));
    connect(_nxtButton,SIGNAL(clicked()),this,SLOT(nxtButtonClicked()));
    //***
    cf = ((CApp*)qApp)->_tjob->_aconfig;
    mf = ((CApp*)qApp)->_tjob->_mconfig;

    setProperty("testFlag",0);
    _channelSample->setMinimum(0);
    _channelSample->setMaximum( cf->type_ana.size()-1 );
    channelChanged(0);
}

void CAdjustTestAnalogWindow::prvButtonClicked()
{
    int c = property("channel").toInt();

    c--;
    if(c<0)
        c = mf->_channel_type.size()-1;

    _channelSample->setValue(c);
    channelChanged(c);
}

void CAdjustTestAnalogWindow::nxtButtonClicked()
{
    int c = property("channel").toInt();

    c++;
    if(c>=mf->_channel_type.size())
        c = 0;

    _channelSample->setValue(c);
    channelChanged(c);
}

void CAdjustTestAnalogWindow::stopButtonClicked()
{
    newMessage(tr("结束模拟量检测"));
    setProperty("testFlag",0);
    _stopButton->setEnabled(false);

    //***仪表关闭功率负载开关
    hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
    b.command[2]=0;

    QByteArray tmp1=QByteArray::fromRawData((const char*)&b,sizeof(b));
    ((CApp*)qApp)->_tjob->sendTestData232(tmp1);

    //***PLC
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    c._12v = 0;
    c._xxv = 2;
    c.ioi[0] = 0;
    c.ioi[1] = 0;

    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
}

void CAdjustTestAnalogWindow::channelChanged(int channel)
{
    vu16 value,sample;

    setProperty("channel",channel);
    setProperty("type",cf->type_ana[property("channel").toInt()]);
    for(int i=0;i<cf->value_ana[channel].size();i++)
    {
        value[i] = cf->value_ana[channel][i];
        sample[i] = cf->sample_ana[channel][i];
    }

    setProperty("id",0);
    setProperty("testFlag",0);
    _resultGridLayout->removeWidget(_currentItem);
    _resultGridLayout->addWidget(_currentItem,1,property("id").toInt()+1);

    updateInterface(cf->value_ana[channel].size(),cf->type_ana[channel],channel,value,sample);
}

void CAdjustTestAnalogWindow::startButtonClicked()
{
    QPushButton* sc=qobject_cast<QPushButton*>(sender());

    setProperty("testFlag",1);
    _stopButton->setEnabled(true);
    for(int i=0;i<_MAX_AADJUST_POINT_;i++)
    {
        if(sc ==_startButton[i])
        {
            setProperty("id",i);
            break;
        }
    }
    startTest();
}

void CAdjustTestAnalogWindow::actualValueChanged(int index)
{
    QSpinBox* sc=qobject_cast<QSpinBox*>(sender());

    for(int i=0;i<_MAX_AADJUST_POINT_;i++)
    {
        if(sc ==_actualValue[i])
        {
            setProperty("id",i);
            break;
        }
    }
    startTest();
}

void CAdjustTestAnalogWindow::startTest()
{
    setProperty("testFlag",1);
    _stopButton->setEnabled(true);

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
    setProperty("testFlag",1);
    int val = _actualValue[property("id").toInt()]->value();
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    setMap(&c,val);

    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

    sendFrame(property("type").toInt(),property("channel").toInt());
    _resultGridLayout->removeWidget(_currentItem);
    _resultGridLayout->addWidget(_currentItem,1,property("id").toInt()+1);
}

void CAdjustTestAnalogWindow::sendFrame(int type,int channel)
{
    adjustCommandFrame_0x40 b={_METER_FRAME_COMMAND_ADJUST_,0x00};

    if(type == ATYPE_CURRENT)
        b.command = 0x5;
    else if(type == ATYPE_RESISTOR)
        b.command = 0x6;
    else if(type == ATYPE_VOLTAGE)
        b.command = 0x7;
    else if(type == ATYPE_POWER)
        b.command = 0x8;

    b.channel = findChannelIndex(type,channel);

    QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
    ((CApp*)qApp)->_tjob->sendTestData232(tmp);
}

void CAdjustTestAnalogWindow::sendBackData485(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        PLCCResponseFrame_0x81 rt = frame81FromData(a);
    }
}

void CAdjustTestAnalogWindow::sendBackData232(CDataFrame a)
{
    if(property("testFlag").toBool()||property("read").toInt()!=-1)
    {
        QByteArray d = a.getData();

        if(a._command == _METER_FRAME_INIT_PRESPONSE_ADJUST_)
        {
            if(property("read").toInt()==1)
            {
                adjustInitPResponseFrame_0x51 rt = frame51FromData(d);

                int num = rt.num_analog[property("channel").toInt()];

                adjustSaveParaFrame_0x46_0x49 b={0};
                if(property("type").toInt() == ATYPE_CURRENT)
                    b.cmd = _METER_FRAME_SAVE_PARA_CURRENT_ADJUST_;
                if(property("type").toInt() == ATYPE_RESISTOR)
                    b.cmd = _METER_FRAME_SAVE_PARA_RESISTOR_ADJUST_;
                if(property("type").toInt() == ATYPE_VOLTAGE)
                    b.cmd = _METER_FRAME_SAVE_PARA_VOLTAGE_ADJUST_;
                if(property("type").toInt() == ATYPE_POWER)
                    b.cmd = _METER_FRAME_SAVE_PARA_POWER_ADJUST_;

                b.command1 = (0x01<<4);
                b.command2 = num<<4 | findChannelIndex(property("type").toInt(),property("channel").toInt());

                QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
                ((CApp*)qApp)->_tjob->sendTestData232(tmp);
            }
            else if(property("read").toInt()==0)
            {
                //***保存到结构
                for(int i=0;i<property("number").toInt();i++)
                {
                    cf->value_ana[property("channel").toInt()][i] = _actualValue[i]->text().toInt();
                    cf->sample_ana[property("channel").toInt()][i] = _sampleValue[i]->text().toInt();
                }

                adjustSaveParaFrame_0x46_0x49 b={0x00};
                if(property("type").toInt() == ATYPE_CURRENT)
                    b.cmd = _METER_FRAME_SAVE_PARA_CURRENT_ADJUST_;
                if(property("type").toInt() == ATYPE_RESISTOR)
                    b.cmd = _METER_FRAME_SAVE_PARA_RESISTOR_ADJUST_;
                if(property("type").toInt() == ATYPE_VOLTAGE)
                    b.cmd = _METER_FRAME_SAVE_PARA_VOLTAGE_ADJUST_;
                if(property("type").toInt() == ATYPE_POWER)
                    b.cmd = _METER_FRAME_SAVE_PARA_POWER_ADJUST_;

                b.command1 = (0x02<<4);
                b.command2 = (property("number").toInt())<<4 | findChannelIndex(property("type").toInt()
                                                                                       ,property("channel").toInt());
                QByteArray tmp;
                for(int i=0;i<property("number").toInt();i++)
                {
                    int val=0;
                    if(property("type").toInt() == ATYPE_CURRENT)
                        val = cf->value_ana[property("channel").toInt()][i]*100;
                    if(property("type").toInt() == ATYPE_RESISTOR)
                        val = cf->value_ana[property("channel").toInt()][i]*10;
                    if(property("type").toInt() == ATYPE_VOLTAGE)
                        val = cf->value_ana[property("channel").toInt()][i]*10;
                    if(property("type").toInt() == ATYPE_POWER)
                        val = cf->value_ana[property("channel").toInt()][i]/10;

                    tmp = arrangeByte(val,2);
                    b.data[i*4] = tmp[0];
                    b.data[i*4+1] = tmp[1];

                    tmp = arrangeByte(cf->sample_ana[property("channel").toInt()][i],2);
                    b.data[i*4+2] = tmp[0];
                    b.data[i*4+3] = tmp[1];
                }

                tmp=QByteArray::fromRawData((const char*)&b,sizeof(b)-(_MAX_AADJUST_POINT_-property("number").toInt())*4);
                ((CApp*)qApp)->_tjob->sendTestData232(tmp);
            }
        }
        else if(a._command == _METER_FRAME_RSAVE_PARA_CURRENT_ADJUST_ || a._command == _METER_FRAME_RSAVE_PARA_RESISTOR_ADJUST_
                || a._command == _METER_FRAME_RSAVE_PARA_VOLTAGE_ADJUST_ ||
                a._command == _METER_FRAME_RSAVE_PARA_POWER_ADJUST_)
        {
            if(property("read").toInt()==1)
            {
                adjustSaveParaFrame_0x56_0x59 rt = frame56_59FromData(d);
                if(rt.num <= _MAX_AADJUST_POINT_ && rt.num >0)
                {
                    vu16 value,sample;
                    int type;

                    if(a._command == _METER_FRAME_RSAVE_PARA_CURRENT_ADJUST_)
                        type = 0;
                    else if (a._command == _METER_FRAME_RSAVE_PARA_RESISTOR_ADJUST_)
                        type = 1;
                    else if (a._command == _METER_FRAME_RSAVE_PARA_RESISTOR_ADJUST_)
                        type = 2;
                    else if (a._command == _METER_FRAME_RSAVE_PARA_POWER_ADJUST_)
                        type = 3;

                    for(int j=0;j<rt.num;j++)
                    {
                        if(a._command == _METER_FRAME_RSAVE_PARA_CURRENT_ADJUST_)
                            value[j] = rt.value[j+rt.start]/100;
                        else if (a._command == _METER_FRAME_RSAVE_PARA_RESISTOR_ADJUST_)
                            value[j] = rt.value[j+rt.start]/10;
                        else if (a._command == _METER_FRAME_RSAVE_PARA_VOLTAGE_ADJUST_)
                            value[j] = rt.value[j+rt.start]/10;
                        else if (a._command == _METER_FRAME_RSAVE_PARA_POWER_ADJUST_)
                            value[j] = rt.value[j+rt.start]*10; //(mA)
                        sample[j] = rt.sample[j+rt.start];
                    }

                    updateInterface(rt.num,type,findChannelIndexReverse(type,rt.channel),value,sample);
                }
                else
                {
                    setProperty("read",-1);
                    newMessage(tr("参数读取错误，请确定仪表内有正确参数！"),0);
                }
            }
            else if(property("read").toInt()==0)
            {
                _savaStatusLabel->setText(tr("模拟通道 ") + property("channel").toString() + tr(" 的参数保存结束！"));
                setProperty("read",-1);

                for(int i=0;i<_MAX_AADJUST_POINT_;i++)
                {
                    _actualValue[i]->setEnabled(true);
                    _startButton[i]->setEnabled(true);
                    _savaAllAdjustButton->setEnabled(true);
                    _writeButton->setEnabled(true);
                }
                _prvButton->setEnabled(true);
                _nxtButton->setEnabled(true);
                _channelSample->setEnabled(true);
            }
        }
        else if(a._command == _METER_FRAME_CRESPONSE_ADJUST_)
        {
            adjustCResponseFrame_0x50 rt = frame50FromData(d);
            _sampleValue[property("id").toInt()]->setText(QString::number(rt.sample));
        }
        else if(a._command == _METER_FRAME_RSAVE_ADJUST_)
        {
            adjustRSaveFrame_0x5F rt = frame5FFromData(d);
            _savaStatusLabel->setText(tr("参数固化入仪表成功！用时 ") + rt.time + tr(" ms"));
        }
    }
}

void CAdjustTestAnalogWindow::setMap(PLCCommandFrame_0x80* a,int val)
{
    if(property("type").toInt()==ATYPE_CURRENT)
    {
        int k = findChannelIndex(property("type").toInt(),property("channel").toInt());
        int j = mf->_map_current[k];
        a->curout[j] = val*10;
    }
    else if(property("type").toInt()==ATYPE_RESISTOR)
    {
        int k = findChannelIndex(property("type").toInt(),property("channel").toInt());
        int j = mf->_map_resistor[k];
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
            int j = mf->_map_voltage[k];
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
        int j = mf->_map_power[k];

        QByteArray b = setupLoader(val,property("type").toInt(),k);
        a->ioi[0] = b[0];
        a->ioi[1] = b[1];
    }
}
bool CAdjustTestAnalogWindow::findMeterVoltageSampleChannel(int type,int channel)
{
    int c = findChannelIndex(type,channel);
    return (mf->_map_meter_voltage == mf->_map_voltage[c]);
}

QByteArray CAdjustTestAnalogWindow::setupLoader(int val,int type,int channel)
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

int CAdjustTestAnalogWindow::findChannelIndex(int t,int c)
{
    int index=0;

    for(int i=0;i<cf->type_ana.size();i++)
    {
        if(t==cf->type_ana[i]&&i<c)
            index++;
    }

    return index;
}

int CAdjustTestAnalogWindow::findChannelIndexReverse(int t,int c)
{
    int index=0;

    for(int i=0;i<cf->type_ana.size();i++)
    {
        if(t==cf->type_ana[i])
        {
            if( index == c)
                return i;
            else
                index++;
        }
    }

    return -1;
}



void CAdjustTestAnalogWindow::saveAllAdjustButtonClicked()
{
    setProperty("read",0);
    setProperty("testFlag",0);

    adjustInitParaFrame_0x41 b={_METER_FRAME_INIT_PARA_ADJUST_,0x00};
    b.type = 0x2;
    writeAdjustInitFrame(b);

    _savaAllAdjustButton->setEnabled(false);
    _savaStatusLabel->setText(tr("参数保存中..."));

    for(int i=0;i<_MAX_AADJUST_POINT_;i++)
    {
        _actualValue[i]->setEnabled(false);
        _startButton[i]->setEnabled(false);
        _savaAllAdjustButton->setEnabled(false);
        _writeButton->setEnabled(false);
        _prvButton->setEnabled(false);
        _nxtButton->setEnabled(false);
        _channelSample->setEnabled(false);
    }

    QByteArray tmp;
    tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
    ((CApp*)qApp)->_tjob->sendTestData232(tmp);
}

void CAdjustTestAnalogWindow::writeButtonClicked()
{
    int iRet = QMessageBox::question(this, "警告", "确定把模拟量调校参数（同时包括其他所有调校项目的参数）写入仪表？",
                                     QMessageBox::Yes, QMessageBox::No);
    if(iRet == QMessageBox::No)
        return;
    else
    {
        setProperty("testFlag",0);
        setProperty("read",0);
        adjustSaveFrame_0x4F b={_METER_FRAME_SAVE_ADJUST_,0x00};

        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        cf->writeFile( ((CApp*)qApp)->_config->_afile );
        setProperty("testFlag",0);
    }
}

void CAdjustTestAnalogWindow::readButtonClicked()
{
    int iRet = QMessageBox::question(this,tr("警告"), tr("确定把仪表模拟量通道 ") + property("channel").toString() + tr(" 的调校参数读入？"),
                                     QMessageBox::Yes, QMessageBox::No);
    if(iRet == QMessageBox::No)
        return;
    else
    {
        setProperty("read",1);
        setProperty("testFlag",0);

        adjustInitParaFrame_0x41 b={_METER_FRAME_INIT_PARA_ADJUST_,0x00};
        b.type = 0x01;

        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _savaStatusLabel->setText(tr("参数读取中..."));

        for(int i=0;i<_MAX_AADJUST_POINT_;i++)
        {
            _actualValue[i]->setEnabled(false);
            _theorySampleValue[i]->setText("");
            _startButton[i]->setEnabled(false);
            _sampleValue[i]->setText("");
            _savaAllAdjustButton->setEnabled(false);
            _writeButton->setEnabled(false);
            _prvButton->setEnabled(false);
            _nxtButton->setEnabled(false);
            _channelSample->setEnabled(false);
            _readButton->setEnabled(false);
        }
    }
}

void CAdjustTestAnalogWindow::updateInterface(int number,int type,int channel,vu16 value,vu16 sample)
{
    int i;
    setProperty("number",number);
    setProperty("testFlag",0);
    setProperty("channel", channel);
    setProperty("type",type);

    _analogType->setCurrentIndex(type);

    _resultGridLayout->removeWidget(_currentItem);
    _resultGridLayout->addWidget(_currentItem,1,1);

    for(i=0;i<number;i++)
    {
        _startButton[i]->setEnabled(true);
        _actualValue[i]->setEnabled(true);
        _sampleValue[i]->setText(QString::number(sample[i]));
        _theorySampleValue[i]->setText("");
        _savedSampleValue[i]->setText("");

        _actualValue[i]->setMaximum(1000000);
        _actualValue[i]->setValue(value[i]);
        _savedSampleValue[i]->setText(QString::number(sample[i]));

        if(type==ATYPE_CURRENT)
        {
            _theorySampleValue[i]->setText( QString::number(
                                                getTheorySample(mf->range_current_min,
                                                            mf->range_current_max,
                                                            value[i]*100,
                                                            4096)
                                                )
                                            );
            _actualValue[i]->setMinimum(4);
            _actualValue[i]->setMaximum(20);
            qobject_cast<QLabel*>(_resultGridLayout->itemAtPosition(2, 0)->widget())->setText( tr("模拟量实际值(mA)")) ;
        }
        else if(type==ATYPE_RESISTOR)
        {
            _theorySampleValue[i]->setText( QString::number(
                                                getTheorySample(mf->range_resistor_min,
                                                            mf->range_resistor_max,
                                                            value[i]*10,
                                                            4096)
                                                )
                                            );
            _actualValue[i]->setMinimum(0);
            _actualValue[i]->setMaximum(500);
            qobject_cast<QLabel*>(_resultGridLayout->itemAtPosition(2, 0)->widget())->setText( tr("模拟量实际值(Ω)")) ;
        }
        else if(type==ATYPE_VOLTAGE)
        {
            _theorySampleValue[i]->setText( QString::number(
                                                getTheorySample(mf->range_voltage_min,
                                                            mf->range_voltage_max,
                                                            value[i]*10,
                                                            4096)
                                                )
                                            );
            _actualValue[i]->setMinimum(0);
            _actualValue[i]->setMaximum(36);
            qobject_cast<QLabel*>(_resultGridLayout->itemAtPosition(2, 0)->widget())->setText( tr("模拟量实际值(V)")) ;
        }
        else if(type==ATYPE_POWER)
        {
            _theorySampleValue[i]->setText( QString::number(
                                                getTheorySample(mf->range_power_min,
                                                            mf->range_power_max,
                                                            value[i]/10,
                                                            4096)
                                                )
                                            );
            _actualValue[i]->setMinimum(0);
            _actualValue[i]->setMaximum(3000);
            qobject_cast<QLabel*>(_resultGridLayout->itemAtPosition(2, 0)->widget())->setText( tr("模拟量实际值(mA)")) ;
        }
    }

    for(i=number;i<_MAX_AADJUST_POINT_;i++)
    {
        _startButton[i]->setEnabled(false);
        _actualValue[i]->setEnabled(false);
        _theorySampleValue[i]->setText("");
        _sampleValue[i]->setText("");
        _savedSampleValue[i]->setText("");
    }

    if(property("read").toInt()==1)
    {
        _savaStatusLabel->setText(tr("参数读取结束！"));

        for(i=0;i<number;i++)
        {
            _startButton[i]->setEnabled(true);
        }
        _savaAllAdjustButton->setEnabled(true);
        _writeButton->setEnabled(true);
        _readButton->setEnabled(true);
        _prvButton->setEnabled(true);
        _nxtButton->setEnabled(true);
        _channelSample->setEnabled(true);
        setProperty("read",-1);
    }
}

void CAdjustTestAnalogWindow::writeAdjustInitFrame(adjustInitParaFrame_0x41 &b)
{
    b.num_meter[0] = cf->value_meter["oil"].size();
    b.num_meter[1] = cf->value_meter["rspeed"].size();
    b.num_meter[2] = cf->value_meter["wtemp"].size();
    b.num_meter[3] = cf->value_meter["speed"].size();

    for(int i=0;i<cf->type_ana.size();i++)
        b.num_analog[i] = cf->value_ana[i].size();

    b.rcof = cf->oil_rcof;
    QByteArray dd = getDateBCD();
    b.date[0] = dd[0];
    b.date[1] = dd[1];
    b.date[2] = dd[2];
    b.date[3] = dd[3];
}

/*快捷键*/
void CAdjustTestAnalogWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {

        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_A)
        {
            setProperty("testFlag",1);
            if(property("id").toInt() == 0)
            {
                setProperty("id", property("number").toInt() - 1);
            }
            else
                setProperty("id", property("id").toInt() - 1);
            _actualValue[property("id").toInt()]->setFocus();
            startTest();
            _resultGridLayout->removeWidget(_currentItem);
            _resultGridLayout->addWidget(_currentItem, 1, property("id").toInt() + 1);

        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_D)
        {
            setProperty("testFlag",1);
            if(property("id").toInt() == property("number").toInt() - 1)
            {
                setProperty("id", 0);
            }
            else
                setProperty("id", property("id").toInt() + 1);
            _actualValue[property("id").toInt()]->setFocus();
            startTest();
            _resultGridLayout->removeWidget(_currentItem);
            _resultGridLayout->addWidget(_currentItem, 1, property("id").toInt() + 1);
        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_W)
        {

        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_S)
        {

        }
        else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S)
        {
            saveAllAdjustButtonClicked();
        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_R)
        {
            readButtonClicked();
        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Z)
        {

        }
    }
}
