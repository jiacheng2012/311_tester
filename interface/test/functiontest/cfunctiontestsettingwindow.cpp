#include "cfunctiontestsettingwindow.h"
#include "capp.h"

CFunctionTestSettingWindow::CFunctionTestSettingWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("read",-1);

    //***UI CONPONENTS
    _accumulatedMileage = new QSpinBox; //累计里程
    _accumulatedMileage->setMaximum(999999999);
    _simpleMileage = new QSpinBox; //小计里程
    _simpleMileage->setMaximum(999999999);
    _ledlight = new QSpinBox; //背光亮度
    _ledlight->setMinimum(0);
    _ledlight->setMaximum(100);
    _lcdbacklight = new QSpinBox; //指示灯亮度
    _lcdbacklight->setMinimum(0);
    _lcdbacklight->setMaximum(100);

    _lowspeedFanDiff = new QSpinBox; //低速风扇回差
    _lowspeedFanDiff->setMaximum(255);
    _highspeedFanDiff = new QSpinBox; //高速风扇回差
    _highspeedFanDiff->setMaximum(255);

    _warn_coolingWaterTemperature = new QSpinBox; //冷却水温报警点
    _warn_coolingWaterTemperature->setMaximum(255);
    _warn_oilpressure = new QSpinBox; //机油压力报警点
    _warn_oilpressure->setMaximum(255);
    _warn_oil = new QSpinBox; //燃油报警点
    _warn_oil->setMinimum(0);
    _warn_oil->setMaximum(100);
    _warn_lowpressure = new QSpinBox; //气压低报警点
    _warn_lowpressure->setMaximum(10000);

    _lowspeedFanInGasTemperature = new QSpinBox; //低速风扇进气温度参数
    _lowspeedFanInGasTemperature->setMaximum(300);
    _highspeedFanInGasTemperature = new QSpinBox; //高速风扇进气温度参数
    _highspeedFanInGasTemperature->setMaximum(300);
    _lowspeedFan_coolingWaterTemperature = new QSpinBox; //低速风扇冷却水温参数
    _lowspeedFan_coolingWaterTemperature->setMaximum(300);
    _highspeedFan_coolingWaterTemperature = new QSpinBox; //高速风扇冷却水温参数
    _highspeedFan_coolingWaterTemperature->setMaximum(300);

    _pulseCount = new QSpinBox; //脉冲计数
    _pulseCount->setMaximum(999999999);
    _engineWorkTime_Hour = new QSpinBox; //发动机工作时间(时)
    _engineWorkTime_Hour->setMaximum(999999999);
    _engineWorkTime_Minute = new QSpinBox; //发动机工作时间(分)
    _engineWorkTime_Minute->setMaximum(60);
    _engineWorkTime_Minute->setMinimum(0);
    _engineWorkTime_Second = new QSpinBox; //发动机工作时间(秒)
    _engineWorkTime_Second->setMaximum(60);
    _engineWorkTime_Second->setMinimum(0);

    _writeButton = new QPushButton(tr("保存参数"));
    _writeButton->setStyleSheet("min-width:100px;");
    _readButton = new QPushButton(tr("读取参数"));
    _readButton->setStyleSheet("min-width:100px;");

    _statusLabel = new QLabel(tr("操作状态"));
    _statusLabel->setStyleSheet("background:#EFF7FD;color:#1E50A2;font:bold 16px;min-width:300px;max-height:30px;");
    _statusLabel->setAlignment(Qt::AlignCenter);
    //***Layout
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    //QLabel* title = new QLabel(tr("仪表功能参数设置"));
    //title->setStyleSheet("background:#EFF7FD;color:#1E50A2;font:bold 16px;min-width:300px;max-height:30px;");
    //title->setAlignment(Qt::AlignCenter);
    QHBoxLayout *group1_h1 = new QHBoxLayout;

    QGroupBox *group1_1 = new QGroupBox(tr("里程参数"));
    QGridLayout *grid1_1 = new QGridLayout(group1_1);
    grid1_1->addWidget(new QLabel(tr("累计里程"),0,0));
    grid1_1->addWidget(new QLabel(tr("小计里程"),0,0));
    grid1_1->addWidget(_accumulatedMileage,0,1);
    grid1_1->addWidget(_simpleMileage,1,1);
    grid1_1->addWidget(new QLabel(tr("m")),0,2);
    grid1_1->addWidget(new QLabel(tr("m")),1,2);
    QGroupBox *group1_2 = new QGroupBox(tr("LED亮度"));
    QGridLayout *grid1_2 = new QGridLayout(group1_2);
    grid1_2->addWidget(new QLabel(tr("背光亮度")),0,0);
    grid1_2->addWidget(new QLabel(tr("指示灯亮度")),1,0);
    grid1_2->addWidget(_lcdbacklight,0,1);
    grid1_2->addWidget(_ledlight,1,1);
    grid1_2->addWidget(new QLabel(tr("%")),0,2);
    grid1_2->addWidget(new QLabel(tr("%")),1,2);
    QGroupBox *group1_3 = new QGroupBox(tr("风扇回差"));
    QGridLayout *grid1_3 = new QGridLayout(group1_3);
    grid1_3->addWidget(new QLabel(tr("低速风扇回差")),0,0);
    grid1_3->addWidget(new QLabel(tr("高速风扇回差")),1,0);
    grid1_3->addWidget(_lowspeedFanDiff,0,1);
    grid1_3->addWidget(_highspeedFanDiff,1,1);
    group1_h1->addWidget(group1_1);
    group1_h1->addWidget(group1_2);
    group1_h1->addWidget(group1_3);

    QHBoxLayout *group1_h2 = new QHBoxLayout;
    QGroupBox *group2_1 = new QGroupBox(tr("报警点"));
    QGridLayout *grid2_1 = new QGridLayout(group2_1);
    grid2_1->addWidget(new QLabel(tr("冷却水温报警点")),0,0);
    grid2_1->addWidget(new QLabel(tr("机油压力报警点")),1,0);
    grid2_1->addWidget(new QLabel(tr("燃油报警点")),2,0);
    grid2_1->addWidget(new QLabel(tr("气压低报警点")),3,0);
    grid2_1->addWidget(_warn_coolingWaterTemperature,0,1);
    grid2_1->addWidget(_warn_oilpressure,1,1);
    grid2_1->addWidget(_warn_oil,2,1);
    grid2_1->addWidget(_warn_lowpressure,3,1);
    grid2_1->addWidget(new QLabel(tr("℃")),0,2);
    grid2_1->addWidget(new QLabel(tr("kpa")),1,2);
    grid2_1->addWidget(new QLabel(tr("%")),2,2);
    grid2_1->addWidget(new QLabel(tr("kpa")),3,2);

    QGroupBox *group2_2 = new QGroupBox(tr("温度参数"));
    QGridLayout *grid2_2 = new QGridLayout(group2_2);
    grid2_2->addWidget(new QLabel(tr("低速风扇进气温度参数")),0,0);
    grid2_2->addWidget(new QLabel(tr("高速风扇进气温度参数")),1,0);
    grid2_2->addWidget(new QLabel(tr("低速风扇冷却水温参数")),2,0);
    grid2_2->addWidget(new QLabel(tr("高速风扇冷却水温参数")),3,0);
    grid2_2->addWidget(_lowspeedFanInGasTemperature,0,1);
    grid2_2->addWidget(_highspeedFanInGasTemperature,1,1);
    grid2_2->addWidget(_lowspeedFan_coolingWaterTemperature,2,1);
    grid2_2->addWidget(_highspeedFan_coolingWaterTemperature,3,1);
    grid2_2->addWidget(new QLabel(tr("℃")),0,2);
    grid2_2->addWidget(new QLabel(tr("℃")),1,2);
    grid2_2->addWidget(new QLabel(tr("℃")),2,2);
    grid2_2->addWidget(new QLabel(tr("℃")),3,2);

    QGroupBox *group2_3 = new QGroupBox(tr("其它参数"));
    QGridLayout *grid2_3 = new QGridLayout(group2_3);
    grid2_3->addWidget(new QLabel(tr("脉冲计数")),0,0);
    grid2_3->addWidget(new QLabel(tr("发动机工作时间")),1,0);

    grid2_3->addWidget(_pulseCount,0,1);
    grid2_3->addWidget(_engineWorkTime_Hour,1,1);
    grid2_3->addWidget(_engineWorkTime_Minute,2,1);
    grid2_3->addWidget(_engineWorkTime_Second,3,1);
    grid2_3->addWidget(new QLabel(tr("个")),0,2);
    grid2_3->addWidget(new QLabel(tr("时")),1,2);
    grid2_3->addWidget(new QLabel(tr("分")),2,2);
    grid2_3->addWidget(new QLabel(tr("秒")),3,2);

    group1_h2->addWidget(group2_1);
    group1_h2->addWidget(group2_2);
    group1_h2->addWidget(group2_3);

    _language = new QComboBox; //低速风扇进气温度参数
    _language->addItem(tr("中文"));
    _language->addItem(tr("英文"));
    _theme = new QComboBox;
    _theme->addItem(tr("蓝色"));
    _theme->addItem(tr("绿色"));
    _theme->addItem(tr("红色"));
    _passwd = new QLineEdit;
    _passwd->setMaxLength(10);
    _version = new QComboBox;
    _version->addItem(tr("75吨欧三"));
    _version->addItem(tr("75吨欧四"));
    _version->addItem(tr("100吨欧三"));
    _version->addItem(tr("100吨欧四"));
    _allowZero = new QComboBox;
    _allowZero->addItem(tr("不允许"));
    _paramSetting = new QComboBox;
    _paramSetting->addItem(tr("固化解除"));
    _paramSetting->addItem(tr("固化锁定"));

    QHBoxLayout *group1_h3 = new QHBoxLayout;
    QGroupBox *group3_1 = new QGroupBox(tr("用户/界面"));
    QGridLayout *grid3_1 = new QGridLayout(group3_1);
    grid3_1->addWidget(new QLabel(tr("语言选择")),0,0);
    grid3_1->addWidget(new QLabel(tr("界面风格")),1,0);
    grid3_1->addWidget(new QLabel(tr("管理密码")),2,0);
    grid3_1->addWidget(new QLabel(tr("软件版本")),3,0);
    grid3_1->addWidget(new QLabel(tr("总计里程清零")),4,0);
    grid3_1->addWidget(new QLabel(tr("参数固化设置")),5,0);
    grid3_1->addWidget(_language,0,1);
    grid3_1->addWidget(_theme,1,1);
    grid3_1->addWidget(_passwd,2,1);
    grid3_1->addWidget(_version,3,1);
    grid3_1->addWidget(_allowZero,4,1);
    grid3_1->addWidget(_paramSetting,5,1);

    _loilType = new QComboBox;
    _loilType->addItem(tr("相对压力"));
    _loilType->addItem(tr("绝对压力"));
    _brakeType = new QComboBox;
    _brakeType->addItem(tr("开关量"));
    _brakeType->addItem(tr("模拟量"));
    _brakeValid = new QComboBox;
    _brakeValid->addItem(tr("高电平"));
    _brakeValid->addItem(tr("低电平"));
    _engineType = new QLineEdit;
    _engineType->setMaxLength(2);
    _gearType = new QLineEdit;
    _gearType->setMaxLength(2);

    QGroupBox *group3_2 = new QGroupBox(tr("电气参数"));
    QGridLayout *grid3_2 = new QGridLayout(group3_2);
    grid3_2->addWidget(new QLabel(tr("机油压力类型")),0,0);
    grid3_2->addWidget(new QLabel(tr("排气制动端子类型")),1,0);
    grid3_2->addWidget(new QLabel(tr("排气制动电平类型")),2,0);
    grid3_2->addWidget(new QLabel(tr("发动机类型")),3,0);
    grid3_2->addWidget(new QLabel(tr("变速箱类型")),4,0);
    grid3_2->addWidget(_loilType,0,1);
    grid3_2->addWidget(_brakeType,1,1);
    grid3_2->addWidget(_brakeValid,2,1);
    grid3_2->addWidget(_engineType,3,1);
    grid3_2->addWidget(_gearType,4,1);

    _year = new QSpinBox;
    _year->setMinimum(2000);
    _year->setMaximum(2030);
    _month = new QSpinBox;
    _month->setMinimum(1);
    _month->setMaximum(12);
    _day = new QSpinBox;
    _day->setMinimum(1);
    _day->setMaximum(31);
    _hour = new QSpinBox;
    _hour->setMinimum(0);
    _hour->setMaximum(60);
    _minute = new QSpinBox;
    _minute->setMinimum(0);
    _minute->setMaximum(60);
    _second = new QSpinBox;
    _second->setMinimum(0);
    _second->setMaximum(60);
    _timeSync = new QCheckBox(tr("同步电脑时间"));
    _timeSync->setChecked(true);

    QGroupBox *group3_3 = new QGroupBox(tr("仪表时间"));
    QGridLayout *grid3_3 = new QGridLayout(group3_3);
    grid3_3->addWidget(new QLabel(tr("年")),0,0);
    grid3_3->addWidget(new QLabel(tr("月")),1,0);
    grid3_3->addWidget(new QLabel(tr("日")),2,0);
    grid3_3->addWidget(new QLabel(tr("时")),3,0);
    grid3_3->addWidget(new QLabel(tr("分")),4,0);
    grid3_3->addWidget(new QLabel(tr("秒")),5,0);

    grid3_3->addWidget(_year,0,1);
    grid3_3->addWidget(_month,1,1);
    grid3_3->addWidget(_day,2,1);
    grid3_3->addWidget(_hour,3,1);
    grid3_3->addWidget(_minute,4,1);
    grid3_3->addWidget(_second,5,1);
    grid3_3->addWidget(_timeSync,6,0,1,2);

    _oilBias = new QSpinBox;
    _oilBias->setMinimum(-2000);
    _oilBias->setMaximum(2000);
    _rspeedBias = new QSpinBox;
    _rspeedBias->setMinimum(-2000);
    _rspeedBias->setMaximum(2000);
    _wtempBias = new QSpinBox;
    _wtempBias->setMinimum(-2000);
    _wtempBias->setMaximum(2000);
    _speedBias = new QSpinBox;
    _speedBias->setMinimum(-2000);
    _speedBias->setMaximum(2000);
    QGroupBox *group3_4 = new QGroupBox(tr("表针偏差"));
    QGridLayout *grid3_4 = new QGridLayout(group3_4);
    grid3_4->addWidget(new QLabel(tr("燃油表零点偏移")),0,0);
    grid3_4->addWidget(new QLabel(tr("转速表零点偏移")),1,0);
    grid3_4->addWidget(new QLabel(tr("水温表零点偏移")),2,0);
    grid3_4->addWidget(new QLabel(tr("速度表零点偏移")),3,0);

    grid3_4->addWidget(_oilBias,0,1);
    grid3_4->addWidget(_rspeedBias,1,1);
    grid3_4->addWidget(_wtempBias,2,1);
    grid3_4->addWidget(_speedBias,3,1);


    group1_h3->addWidget(group3_1);
    group1_h3->addWidget(group3_2);
    group1_h3->addWidget(group3_3);
    group1_h3->addWidget(group3_4);

    //***文件区
    _tip = new QLabel(tr("请检查参数，然后进行下载和读取参数操作。"));
    _tip->setStyleSheet("background:#CCFF99;font:bold 15px;color:#0099FF;min-width:300px;max-height:30px;");
    _tip->setAlignment(Qt::AlignCenter);
    //***底部 状态
    QHBoxLayout* bottom = new QHBoxLayout;
    bottom->addWidget(_statusLabel);
    bottom->addWidget(_writeButton);
    bottom->addWidget(_readButton);
    bottom->setSpacing(5);
    bottom->setAlignment(Qt::AlignCenter);
   // mainVLayout->addWidget(title,0,Qt::AlignCenter);

    mainVLayout->addLayout(group1_h1);
    mainVLayout->addLayout(group1_h2);
    mainVLayout->addLayout(group1_h3);

    mainVLayout->addWidget(_tip,0,Qt::AlignCenter);
    mainVLayout->addLayout(bottom);
    mainVLayout->setContentsMargins(10,0,10,0);
    mainVLayout->setSpacing(3);
    setLayout(mainVLayout);

    //***signals
    connect(_timeSync,SIGNAL(stateChanged(int)),this,SLOT(timeToggled(int)));
    connect(_readButton,SIGNAL(clicked()),this,SLOT(readButtonClicked()));
    connect(_writeButton,SIGNAL(clicked()),this,SLOT(writeButtonClicked()));
    connect(((CApp*)qApp),SIGNAL(sendBackFunctionTestData_232(CDataFrame)),this,SLOT(sendBackTestData_232(CDataFrame)));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
    _timer = new QTimer;
    connect(_timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    updateInterface();
    _timer->start(1000);
}

void CFunctionTestSettingWindow::timerUpdate()
{
    QDateTime a = QDateTime::currentDateTime();

    _year->setValue(a.date().year());
    _month->setValue(a.date().month());
    _day->setValue(a.date().day());

    _hour->setValue(a.time().hour());
    _minute->setValue(a.time().minute());
    _second->setValue(a.time().second());
}

void CFunctionTestSettingWindow::timeToggled(int st)
{
    if(st == Qt::Unchecked)
        _timer->stop();
    else
        _timer->start(1000);
}

void CFunctionTestSettingWindow::writeButtonClicked()
{
    if(((CApp*)qApp)->_tjob->_status232==0)
    {
       emit newMessage(tr("仪表串口没有连接，请先连接"),1);
       emit nextList(-2);
    }
    else if(((CApp*)qApp)->_tjob->_status485==0)
    {
       emit newMessage(tr("信号箱串口没有连接，请先连接"),1);
       emit nextList(-2);
    }
    else
    {
    //***置高低仪表ON开关量
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        int j = cf->_map_ioin[16];
        int d = (int)(j/4);
        int r = j - d*4;
        c.ioo[d] &= ~(3<<(2*r));
        c.ioo[d] |= 1<<(2*r);
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        QTimer::singleShot(_MS_500_, this, SLOT(delayWrite()));
    }
}

void CFunctionTestSettingWindow::delayWrite()
{
    QByteArray tmp;
    int val;
    MeterSettingFrame_0x15 b={_METER_FRAME_METER_PARA_SAVE_COMMAND_,0x00};
    b.command[0] = 0xff;
    b.command[1] = 0xff;
    b.command[2] = 0xff;
    b.command[3] = 0xff;

    CApp* inst = qobject_cast<CApp*>(qApp);
    QString path = inst->_config->_mfile;
    QSettings *sIniSetting = new QSettings(path, QSettings::IniFormat);

    sIniSetting->setIniCodec("UTF-8");
    //***************
    sIniSetting->setValue("function_param/milage_total",_accumulatedMileage->value());
    tmp=arrangeByte(_accumulatedMileage->value(),4);
    b.milage_all[0] = tmp[0];
    b.milage_all[1] = tmp[1];
    b.milage_all[2] = tmp[2];
    b.milage_all[3] = tmp[3];

    sIniSetting->setValue("function_param/milage_part",_simpleMileage->value());
    tmp=arrangeByte(_simpleMileage->value(),4);
    b.milage_part[0] = tmp[0];
    b.milage_part[1] = tmp[1];
    b.milage_part[2] = tmp[2];
    b.milage_part[3] = tmp[3];

    //*****************
    sIniSetting->setValue("function_param/ledlight",_ledlight->value());
    b.ledlight = _ledlight->value();

    sIniSetting->setValue("function_param/backlight",_lcdbacklight->value());
    b.backlight = _lcdbacklight->value();

    //*****************
    sIniSetting->setValue("function_param/fan_lspeed_diff",_lowspeedFanDiff->value());
    b.fan_lspeed_diff = _lowspeedFanDiff->value();

    sIniSetting->setValue("function_param/fan_hspeed_diff",_highspeedFanDiff->value());
    b.fan_hspeed_diff = _highspeedFanDiff->value();

    //*****************
    sIniSetting->setValue("function_param/w_cwater",_warn_coolingWaterTemperature->value());
    b.w_cwater = _warn_coolingWaterTemperature->value();

    sIniSetting->setValue("function_param/w_lpressure",_warn_oilpressure->value());
    tmp = arrangeByte(_warn_oilpressure->value(),2);
    b.w_lpressure[0] = tmp[0];
    b.w_lpressure[1] = tmp[1];

    sIniSetting->setValue("function_param/w_opressure",_warn_oil->value());
    b.w_opressure = _warn_oil->value();
    sIniSetting->setValue("function_param/w_pressurel",_warn_lowpressure->value());
    tmp = arrangeByte(_warn_lowpressure->value(),2);
    b.w_pressurel[0] = tmp[0];
    b.w_pressurel[1] = tmp[1];

    //*****************
    sIniSetting->setValue("function_param/fan_lspeed_temp",_lowspeedFanInGasTemperature->value());
    b.fan_lspeed_temp = _lowspeedFanInGasTemperature->value();

    sIniSetting->setValue("function_param/fan_hspeed_temp",_highspeedFanInGasTemperature->value());
    b.fan_hspeed_temp = _highspeedFanInGasTemperature->value();

    sIniSetting->setValue("function_param/fan_lspeed_wtemp",_lowspeedFan_coolingWaterTemperature->value());
    b.fan_lspeed_wtemp = _lowspeedFan_coolingWaterTemperature->value();

    sIniSetting->setValue("function_param/fan_hspeed_wtemp",_highspeedFan_coolingWaterTemperature->value());
    b.fan_hspeed_wtemp = _highspeedFan_coolingWaterTemperature->value();

    //*****************
    sIniSetting->setValue("function_param/speed_pulse",_pulseCount->value());
    tmp = arrangeByte(_pulseCount->value(),2);
    b.speed_pulse[0] = tmp[0];
    b.speed_pulse[1] = tmp[1];

    val = _engineWorkTime_Hour->value()*3600 + _engineWorkTime_Minute->value()*60 + _engineWorkTime_Second->value();
    sIniSetting->setValue("function_param/engine_time",val);
    tmp = arrangeByte(val,4);
    b.engine_time[0] = tmp[0];
    b.engine_time[1] = tmp[1];
    b.engine_time[2] = tmp[2];
    b.engine_time[3] = tmp[3];

    //*****************
    sIniSetting->setValue("function_param/language",_language->currentIndex());
    b.language = _language->currentIndex()?0x01:0x00;

    sIniSetting->setValue("function_param/theme",_theme->currentIndex());
    if(_theme->currentIndex()==0)
        val = 0x35a;
    else if(_theme->currentIndex()==1)
        val = 0x50b;
    else if(_theme->currentIndex()==2)
        val = 0xe120;
    tmp = arrangeByte(val,2);
    b.interface_theme[0] = tmp[0];
    b.interface_theme[1] = tmp[1];

    sIniSetting->setValue("function_param/passwd",_passwd->text().toInt());
    QString aaa = _passwd->text();
    tmp = arrangeByte(_passwd->text().toInt(),5);
    b.passwd[0] = aaa[0].toAscii();
    b.passwd[1] = aaa[1].toAscii();
    b.passwd[2] = aaa[2].toAscii();
    b.passwd[3] = aaa[3].toAscii();
    b.passwd[4] = 0;

    sIniSetting->setValue("function_param/version",_version->currentIndex());
    if(_version->currentIndex()==0)
        val = 0x20;
    else if(_version->currentIndex()==1)
        val = 0x21;
    else if(_version->currentIndex()==2)
        val = 0x30;
    else if(_version->currentIndex()==3)
        val = 0x31;
    else
        val = 0x20;
    b.version = val;

    sIniSetting->setValue("function_param/allow_mzero",_allowZero->currentIndex());
    if(_allowZero->currentIndex()==0)
        val = 0x0;
    else if(_allowZero->currentIndex()==1)
        val = 0x1;
    b.allow_mzero = val;

    sIniSetting->setValue("function_param/param_setting",_paramSetting->currentIndex());
    if(_paramSetting->currentIndex()==0)
        val = 0x0;
    else if(_paramSetting->currentIndex()==1)
        val = 0x1;
    b.param_setting = val;

    //*****************
    sIniSetting->setValue("function_param/loil_type",_loilType->currentIndex());
    if(_loilType->currentIndex()==0)
        val = 0x0;
    else if(_loilType->currentIndex()==1)
        val = 0x1;
    b.loil_type = val;

    sIniSetting->setValue("function_param/brake_type",_brakeType->currentIndex());
    if(_brakeType->currentIndex()==0)
        val = 0x0;
    else if(_brakeType->currentIndex()==1)
        val = 0x1;
    b.brake_type = val;

    sIniSetting->setValue("function_param/brake_valid",_brakeValid->currentIndex());
    if(_brakeValid->currentIndex()==0)
        val = 0x0;
    else if(_brakeValid->currentIndex()==1)
        val = 0x1;
    b.brake_valid = val;

    sIniSetting->setValue("function_param/engine_type",_engineType->text());
    if(_engineType->text().toInt()<0 || _engineType->text().toInt()>15)
        val = 0;
    else
        val = _engineType->text().toInt();
    b.engine_type = 0xf0 | val;

    sIniSetting->setValue("function_param/gear_type",_gearType->text());
    if(_gearType->text().toInt()<0 || _gearType->text().toInt()>15)
        val = 0;
    else
        val = _gearType->text().toInt();
    b.gear_type = 0xf0 | val;

    //*****************
    sIniSetting->setValue("function_param/year",_year->value());
    tmp = arrangeByte(_year->value(),2);
    b.year[0] = tmp[0];
    b.year[1] = tmp[1];

    sIniSetting->setValue("function_param/day",_day->value());
    b.day = _day->value();

    sIniSetting->setValue("function_param/hour",_hour->value());
    b.hour = _hour->value();

    sIniSetting->setValue("function_param/minute",_minute->value());
    b.minute = _minute->value();

    sIniSetting->setValue("function_param/second",_second->value());
    b.second = _second->value();

    //*****************
    sIniSetting->setValue("function_param/oil_bias",_oilBias->value());
    tmp = arrangeByte(_oilBias->value(),2);
    b.bstep_oil[0] = tmp[0];
    b.bstep_oil[1] = tmp[1];

    sIniSetting->setValue("function_param/rspeed_bias",_rspeedBias->value());
    tmp = arrangeByte(_rspeedBias->value(),2);
    b.bstep_rspeed[0] = tmp[0];
    b.bstep_rspeed[1] = tmp[1];

    sIniSetting->setValue("function_param/wtemp_bias",_wtempBias->value());
    tmp = arrangeByte(_wtempBias->value(),2);
    b.bstep_wtemp[0] = tmp[0];
    b.bstep_wtemp[1] = tmp[1];

    sIniSetting->setValue("function_param/speed_bias",_speedBias->value());
    tmp = arrangeByte(_speedBias->value(),2);
    b.bstep_speed[0] = tmp[0];
    b.bstep_speed[1] = tmp[1];

    sIniSetting->sync();

    QByteArray tmp1=QByteArray::fromRawData((const char*)&b,sizeof(b));
    ((CApp*)qApp)->_tjob->sendTestData232(tmp1);

    _readButton->setEnabled(false);
    _writeButton->setEnabled(false);

    _statusLabel->setText(tr("仪表参数保存中..."));

    setProperty("read",0);
}

void CFunctionTestSettingWindow::sendBackTestData_232(CDataFrame a)
{
    QByteArray d = a.getData();

    if(a._command == _METER_FRAME_METER_PARA_RSAVE_COMMAND_ && property("read").toInt()==0 )
    {
        setProperty("read",-1);
        newMessage(tr("仪表参数保存成功！"),1);
        _readButton->setEnabled(true);
        _writeButton->setEnabled(true);
        _statusLabel->setText(tr("仪表参数保存成功！"));

        MeterAskFrame_0x16 b;
        b.cmd = _METER_FRAME_METER_PARA_ASK_COMMAND_;
        ((CApp*)qApp)->_tjob->setupSendFrame232(data16FromFrame(b),_MS_500_);

        //***置高仪表ON开关量
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        int j = cf->_map_ioin[16];
        int d = (int)(j/4);
        int r = j - d*4;
        c.ioo[d] &= ~(3<<(2*r));
        c.ioo[d] |= 2<<(2*r);
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
    else if(a._command == _METER_FRAME_METER_PARA_RASK_COMMAND_ && property("read").toInt()==1)
    {
        setProperty("read",-1);
        MeterAskReturnFrame_0x06 rt = frame06FromData(d);

        CApp* inst = qobject_cast<CApp*>(qApp);
        QString path = inst->_config->_mfile;
        int val = 0;
        QSettings *sIniSetting = new QSettings(path, QSettings::IniFormat);

        sIniSetting->setIniCodec("UTF-8");
        //***************
        sIniSetting->setValue("function_param/milage_total",rt.milage_all);
        sIniSetting->setValue("function_param/milage_part",rt.milage_part);

        //*****************
        sIniSetting->setValue("function_param/ledlight",rt.ledlight);
        sIniSetting->setValue("function_param/backlight",rt.backlight);

        //*****************
        sIniSetting->setValue("function_param/fan_lspeed_diff",rt.fan_lspeed_diff);
        sIniSetting->setValue("function_param/fan_hspeed_diff",rt.fan_hspeed_diff);

        //*****************
        sIniSetting->setValue("function_param/w_cwater",rt.w_cwater);
        sIniSetting->setValue("function_param/w_lpressure",rt.w_lpressure);
        sIniSetting->setValue("function_param/w_opressure",rt.w_opressure);
        sIniSetting->setValue("function_param/w_pressurel",rt.w_pressurel);

        //*****************
        sIniSetting->setValue("function_param/fan_lspeed_temp",rt.fan_lspeed_temp);
        sIniSetting->setValue("function_param/fan_hspeed_temp",rt.fan_hspeed_temp);
        sIniSetting->setValue("function_param/fan_lspeed_wtemp",rt.fan_lspeed_wtemp);
        sIniSetting->setValue("function_param/fan_hspeed_wtemp",rt.fan_hspeed_wtemp);

        //*****************
        sIniSetting->setValue("function_param/speed_pulse",rt.speed_pulse);
        sIniSetting->setValue("function_param/engine_time",rt.engine_time);

        //*****************
        sIniSetting->setValue("function_param/language",rt.language);
        if(rt.interface_theme == 0x050b)
            val = 1;
        else if(rt.interface_theme == 0xe120)
            val = 2;
        else
            val = 0;
        sIniSetting->setValue("function_param/theme",val);
        QString aaa = QString(QChar(rt.passwd[0])) + QString(QChar(rt.passwd[1]))  +
                QString(QChar(rt.passwd[2])) + QString(QChar(rt.passwd[3]));
        sIniSetting->setValue("function_param/passwd",aaa);
        if(rt.interface_theme == 0x21)
            val = 1;
        else if(rt.interface_theme == 0x30)
            val = 2;
        else if(rt.interface_theme == 0x31)
            val = 3;
        else
            val = 0;
        sIniSetting->setValue("function_param/version",val);
        sIniSetting->setValue("function_param/allow_mzero",rt.allow_mzero);
        sIniSetting->setValue("function_param/param_setting",rt.param_setting);

        //*****************
        sIniSetting->setValue("function_param/loil_type",rt.loil_type);
        sIniSetting->setValue("function_param/brake_type",rt.brake_type);
        sIniSetting->setValue("function_param/brake_valid",rt.brake_valid);
        val = rt.engine_type & 0xf;
        sIniSetting->setValue("function_param/engine_type",val);
        val = rt.gear_type & 0xf;
        sIniSetting->setValue("function_param/gear_type",val);

        //*****************
        sIniSetting->setValue("function_param/oil_bias",rt.bstep_oil);
        sIniSetting->setValue("function_param/rspeed_bias",rt.bstep_rspeed);
        sIniSetting->setValue("function_param/wtemp_bias",rt.bstep_wtemp);
        sIniSetting->setValue("function_param/speed_bias",rt.bstep_speed);

        sIniSetting->sync();
        _readButton->setEnabled(true);
        _writeButton->setEnabled(true);
        _statusLabel->setText(tr("仪表参数读取成功！"));

        MeterAskFrame_0x16 b;
        b.cmd = _METER_FRAME_METER_PARA_ASK_COMMAND_;
        ((CApp*)qApp)->_tjob->setupSendFrame232(data16FromFrame(b),_MS_500_);

        //***置高仪表ON开关量
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        int j = cf->_map_ioin[16];
        int d = (int)(j/4);
        int r = j - d*4;
        c.ioo[d] &= ~(3<<(2*r));
        c.ioo[d] |= 2<<(2*r);
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        updateInterface();
        newMessage(tr("仪表参数读取成功！"));
    }

}
void CFunctionTestSettingWindow::readButtonClicked()
{
    if(((CApp*)qApp)->_tjob->_status485==0)
    {
       emit newMessage(tr("信号箱串口没有连接，请先连接"),1);
       emit nextList(-2);
    }
    else
    {
        MeterAskFrame_0x16 b;
        b.cmd = _METER_FRAME_METER_PARA_ASK_COMMAND_;
        ((CApp*)qApp)->_tjob->setupSendFrame232(data16FromFrame(b),_MS_1000_);

        _readButton->setEnabled(false);
        _writeButton->setEnabled(false);
        _statusLabel->setText(tr("仪表参数读取中..."));

        setProperty("read",1);
    }
}

void CFunctionTestSettingWindow::updateInterface()
{
    //***导入里程 groupbox配置
    _accumulatedMileage->setValue(cf->_function_param["milage_total"]);
    _simpleMileage->setValue(cf->_function_param["milage_part"]);
    //***导入LED groupbox配置
    _ledlight->setValue(cf->_function_param["ledlight"]);
    _lcdbacklight->setValue(cf->_function_param["backlight"]);
    //***导入风扇回差配置
    _lowspeedFanDiff->setValue(cf->_function_param["fan_lspeed_diff"]);
    _highspeedFanDiff->setValue(cf->_function_param["fan_hspeed_diff"]);
    //***导入报警点 groupbox配置
    _warn_coolingWaterTemperature->setValue(cf->_function_param["w_cwater"]);
    _warn_oilpressure->setValue(cf->_function_param["w_lpressure"]);
    _warn_oil->setValue(cf->_function_param["w_opressure"]);
    _warn_lowpressure->setValue(cf->_function_param["w_pressurel"]);
    //***导入温度点 groupbox配置
    _lowspeedFanInGasTemperature->setValue(cf->_function_param["fan_lspeed_temp"]);
    _highspeedFanInGasTemperature->setValue(cf->_function_param["fan_hspeed_temp"]);
    _lowspeedFan_coolingWaterTemperature->setValue(cf->_function_param["fan_lspeed_wtemp"]);
    _highspeedFan_coolingWaterTemperature->setValue(cf->_function_param["fan_hspeed_wtemp"]);
    //***导入其它配置
    _pulseCount->setValue(cf->_function_param["speed_pulse"]);
    QByteArray tmp = splitTime(cf->_function_param["engine_time"]);
    _engineWorkTime_Hour->setValue(tmp[0]);
    _engineWorkTime_Minute->setValue(tmp[1]);
    _engineWorkTime_Second->setValue(tmp[2]);
    //***导入用户配置
    _language->setCurrentIndex(cf->_function_param["language"]);
    _theme->setCurrentIndex(cf->_function_param["theme"]);
    _passwd->setText(QString::number(cf->_function_param["passwd"]));
    _version->setCurrentIndex(cf->_function_param["version"]);
    _allowZero->setCurrentIndex(cf->_function_param["allow_mzero"]);
    _paramSetting->setCurrentIndex(cf->_function_param["param_setting"]);
    //***导入电气配置
    _loilType->setCurrentIndex(cf->_function_param["loil_type"]);
    _brakeType->setCurrentIndex(cf->_function_param["brake_type"]);
    _brakeValid->setCurrentIndex(cf->_function_param["brake_valid"]);
    _engineType->setText(QString::number(cf->_function_param["engine_type"]));
    _gearType->setText(QString::number(cf->_function_param["gear_type"]));
    //***导入时间配置
    _year->setValue(cf->_function_param["year"]);
    _month->setValue(cf->_function_param["month"]);
    _day->setValue(cf->_function_param["day"]);
    _hour->setValue(cf->_function_param["hour"]);
    _minute->setValue(cf->_function_param["minute"]);
    _second->setValue(cf->_function_param["second"]);
    //***导入指针零点偏移配置
    _oilBias->setValue(cf->_function_param["oil_bias"]);
    _rspeedBias->setValue(cf->_function_param["_rspeed_bias"]);
    _wtempBias->setValue(cf->_function_param["_wtemp_bias"]);
    _speedBias->setValue(cf->_function_param["speed_bias"]);

}
