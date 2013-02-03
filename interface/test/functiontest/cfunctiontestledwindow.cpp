#include "cfunctiontestledwindow.h"
#include "capp.h"

CFunctionTestLEDWindow::CFunctionTestLEDWindow(QWidget *parent) :
    QWidget(parent)
{
    //***UI CONPONENTS
    QGroupBox* _topLevelGroup = new QGroupBox(this);
    QVBoxLayout* _topLevelLayout = new QVBoxLayout(_topLevelGroup);
    QGroupBox *_mainGroup = new QGroupBox(tr("32路LED灯测试"));
    _mainGroup->setStyleSheet("background-color:white");//因为那几张图片背景是白色的，以此达到假透明
    QGridLayout *_topGrid = new QGridLayout(_mainGroup);
    _topGrid->setHorizontalSpacing(20);
    QStringList _bitmapNameList;
    _bitmapNameList <<"左转"<<"充电指示"<<"油压低"<<"水温高"<<"气压低"<<"助力转向"<<"倒车指示"<<"仪表电源"<<"变速箱蠕动"<<"进气预热"<<"后雾灯"<<"远光灯"<<"左右门开报警"<<"右转"
                    <<""<<""<<"水位报警"<<"驻车制动"<<"停车检查"<<"取力指示"<<"变速箱故障"<<"轮间差速"<<"燃油报警"<<"行车制动"<<"轴间差速"<<"排气制动"<<""<<""
                    <<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<"空调开启"<<"xxx"<<""
                    <<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<"高低档切换"<<""<<"";

    _clampComboBox = new QComboBox();        //小灯开关信号
    _clampComboBox->addItem(tr("未知状态"));
    _clampComboBox->addItem(tr("置为有效"));
    _clampComboBox->addItem(tr("置为无效"));
    _frontFoglampComboBox = new QComboBox();      //前雾灯开关信号
    _frontFoglampComboBox->addItem(tr("未知状态"));
    _frontFoglampComboBox->addItem(tr("置为有效"));
    _frontFoglampComboBox->addItem(tr("置为无效"));
    _wipersComboBox = new QComboBox();           //雨刮开关信号
    _wipersComboBox->addItem(tr("未知状态"));
    _wipersComboBox->addItem(tr("雨刮低速有效"));
    _wipersComboBox->addItem(tr("雨刮低速无效"));
    _wipersComboBox->addItem(tr("雨刮高速有效"));
    _wipersComboBox->addItem(tr("雨刮高速无效"));
    _dashboardPowerComboBox = new QComboBox();   //仪表电源开关信号
    _dashboardPowerComboBox->addItem(tr("未知状态"));
    _dashboardPowerComboBox->addItem(tr("置为有效"));
    _dashboardPowerComboBox->addItem(tr("置为无效"));
    _onComboBox = new QComboBox();              //ON档开关信号
    _onComboBox->addItem(tr("未知状态"));
    _onComboBox->addItem(tr("置为有效"));
    _onComboBox->addItem(tr("置为无效"));
    _accComboBox = new QComboBox();            //ACC档开关信号
    _accComboBox->addItem(tr("未知状态"));
    _accComboBox->addItem(tr("置为有效"));
    _accComboBox->addItem(tr("置为无效"));

    _lateralOutLabel = new QLabel; //侧标输出
    _lateralOutLabel->setStyleSheet("border-radius:25px;background:#fff;border:1px solid #666;");
    _lateralOutLabel->setFixedSize(50,50);
    _lateralOutLabel->installEventFilter(this);
    _wipersOutLabel  = new QLabel; //雨刮输出
    _wipersOutLabel ->setStyleSheet("border-radius:25px;background:#fff;border:1px solid #666;");
    _wipersOutLabel->setFixedSize(50,50);
    _wipersOutLabel->installEventFilter(this);
    _powerOutLabel  = new QLabel; //电源输出
    _powerOutLabel->setStyleSheet("border-radius:25px;background:#fff;border:1px solid #666;");
    _powerOutLabel->setFixedSize(50,50);
    _powerOutLabel->installEventFilter(this);

    _statusLabel = new QLabel;
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    _topGrid->addWidget(_statusLabel,0,0,1,14);
    for(int i=0; i < BITMAP_NAME_COUNT; ++i)
    {
        if(_bitmapNameList.at(i)!="")
        {
            QVBoxLayout *_ledStateVLayout = new QVBoxLayout();
            _ledStateBitmaps[i] = new QLabel();
            QLabel *_ledName = new QLabel(_bitmapNameList.at(i));
            _ledName->setAlignment(Qt::AlignCenter);
            _ledStateBitmaps[i]->setScaledContents(true);
            _ledStateBitmaps[i]->setPixmap(QPixmap(":/res/"+_bitmapNameList.at(i)+".bmp"));
            _ledStateBitmaps[i]->setFixedSize(48,50);
            _ledStateBitmaps[i]->installEventFilter(this);
            _ledStateBitmaps[i]->setProperty("picture",true);

            _ledStateLabels[i] = new QLabel;
            _ledStateLabels[i]->setStyleSheet("border-radius:25px;background:#fff;border:1px solid #666;");
            _ledStateLabels[i]->setFixedSize(50,50);

            _ledStateVLayout->addWidget(_ledStateBitmaps[i]);
            _ledStateVLayout->addWidget(_ledStateLabels[i]);
            _ledStateVLayout->addWidget(_ledName);
            _topGrid->addLayout(_ledStateVLayout,(int)(i/14)+1,i - ((int)(i/14))*14);
        }
    }
    _topGrid->addItem(new QSpacerItem(80,70),13,0);
    //***Layout
    QGroupBox *_bottomLeftGroupbox = new QGroupBox(tr("背光状态"),_mainGroup);
    QGridLayout *_bottomLeftGrid = new QGridLayout(_bottomLeftGroupbox);
    _bottomLeftGrid->addWidget(new QLabel(tr("小灯开关信号：")),0,0);
    _bottomLeftGrid->addWidget(_clampComboBox,0,1);
    _bottomLeftGrid->addWidget(new QLabel(tr("前雾灯开关信号：")),1,0);
    _bottomLeftGrid->addWidget(_frontFoglampComboBox,1,1);
    _bottomLeftGrid->addWidget(new QLabel(tr("雨刮开关信号：")),2,0);
    _bottomLeftGrid->addWidget(_wipersComboBox,2,1);
    _bottomLeftGrid->addWidget(new QLabel(tr("仪表电源开关信号：")),3,0);
    _bottomLeftGrid->addWidget(_dashboardPowerComboBox,3,1);
    _bottomLeftGrid->addWidget(new QLabel(tr("ON档开关信号：")),4,0);
    _bottomLeftGrid->addWidget(_onComboBox,4,1);
    _bottomLeftGrid->addWidget(new QLabel(tr("ACC档开关信号：")),5,0);
    _bottomLeftGrid->addWidget(_accComboBox,5,1);
    _bottomLeftGrid->addWidget(_lateralOutLabel,0,2);
    _bottomLeftGrid->addWidget(new QLabel(tr("侧标输出")),1,2);
    _bottomLeftGrid->addWidget(_wipersOutLabel,2,2);
    _bottomLeftGrid->addWidget(new QLabel(tr("雨刮输出")),3,2);
    _bottomLeftGrid->addWidget(_powerOutLabel,4,2);
    _bottomLeftGrid->addWidget(new QLabel(tr("电源输出")),5,2);

    _topLevelLayout->addWidget(_mainGroup);
    QVBoxLayout  *totalVLayout=new QVBoxLayout();
    QScrollArea  *scroll=new QScrollArea();
    scroll->setWidget(_topLevelGroup);
    scroll->setAlignment(Qt::AlignCenter);
    totalVLayout->addWidget(scroll);
    setLayout(totalVLayout);
    _bottomLeftGroupbox->setGeometry(80, 300, 400, 280);


    //***signal
    connect(_startButton,SIGNAL(clicked()),this,SLOT(startButtonClicked()));

    _timer = new QTimer();
    connect(_timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFunctionTestLEDWindow::timerUpdate()
{
    for(int i=0;i<cf->_map_led.size();i++)
        if(cf->_map_led[_cur]!=-1)
            _ledStateLabels[cf->_map_led[_cur]]->setStyleSheet("border-radius:25px;background:#ffffff;border:1px solid #666;");

    _cur++;
    if(_cur>=cf->_map_led.size())
        _cur=0;

    //***PLC
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    if(cf->_map_led_io[_cur]!=-1)
    {
        setIOMap(&c,cf->_map_led_io[_cur]);
    }
    else
    {
        switch(_cur)
        {
        //***油压低
        case 3:
            break;
        //***水温高
        case 5:
            break;
        //***气压低
        case 7:
            break;

        }
    }

    QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

    if(cf->_map_led[_cur]!=-1)
        _ledStateLabels[cf->_map_led[_cur]]->setStyleSheet("border-radius:25px;background:red;border:1px solid #666;");
}

void CFunctionTestLEDWindow::startButtonClicked()
{
    if(!this->property("testFlag").toInt())
    {
        _cur = 0;
        _timer->start(_MS_3000_);
        _startButton->setText(tr("停止检测"));
        emit newMessage(tr("开始LED检测"),0);
        setProperty("testFlag", 1);
    }
    else
    {
        _timer->stop();
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[1],0);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _startButton->setText(tr("开始全部检测"));
        setProperty("testFlag",0);
    }
}

bool CFunctionTestLEDWindow::eventFilter(QObject *obj, QEvent *event)
{
    //***圆圈QLabel 点击事件处理
    if (event->type() == QEvent::MouseButtonPress)
    {
        //***左转
        if(obj == _ledStateBitmaps[0] || obj == _ledStateLabels[0])
        {

        }
        //***充电
        else if(obj == _ledStateBitmaps[1] || obj == _ledStateLabels[1])
        {

        }
        //***助力转向
        else if(obj == _ledStateBitmaps[2] || obj == _ledStateLabels[2])
        {

        }
        //***倒车 led 4
        else if(obj == _ledStateBitmaps[3] || obj == _ledStateLabels[3])
        {

        }
        //***led 5
        else if(obj == _ledStateBitmaps[4] || obj == _ledStateLabels[4])
        {

        }
        //***led 6
        else if(obj == _ledStateBitmaps[5] || obj == _ledStateLabels[5])
        {

        }
        //***led 7
        else if(obj == _ledStateBitmaps[6] || obj == _ledStateLabels[6])
        {

        }

        return true;
    }
    //***后面是鼠标效果处理
    else if(event->type() == QEvent::Enter)
    {
        //***如果鼠标 移动到的是图片
        if(obj->property("picture").toBool())
        {
            (qobject_cast<QLabel*>(obj))->setFixedSize(50,52);//稍微变动高和宽
            (qobject_cast<QLabel*>(obj))->setCursor(Qt::PointingHandCursor);
            return true;
        }
    }
    else if(event->type() == QEvent::Leave)
    {
        if(obj->property("picture").toBool())
        {
            (qobject_cast<QLabel*>(obj))->setFixedSize(48,50);

            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}

void CFunctionTestLEDWindow::setIOMap(PLCCommandFrame_0x80* a,int i)
{
    int j = cf->_map_ioin[i];

    int d = (int)(j/4);
    int r = j - d*4;

    a->ioo[d] &= ~(3<<(2*r));
    if(((CApp*)qApp)->_tjob->getIOInMapVoltage(i))
        a->ioo[d] |= 2<<(2*r);
    else
        a->ioo[d] |= 1<<(2*r);

    //***差分IO
    int kk = ((CApp*)qApp)->_tjob->getIOInMapDifference(i);
    if(kk!=-1)
    {
        int n = (int)(kk/4);
        int o = kk - n*4;

        a->ioo[n] &= ~(3<<(2*o));
        a->ioo[n] |= 1<<(2*o);
    }

}
