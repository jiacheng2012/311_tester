#include "cfulltestledwindow.h"
#include "protocol/protocol.h"
#include "capp.h"

CFullTestLEDWindow::CFullTestLEDWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);

    cf = ((CApp*)qApp)->_tjob->_mconfig;

    QVBoxLayout *top = new QVBoxLayout();

    //***UI CONPONENTS
    _statusLabel = new QLabel;
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");
    QGroupBox *_mainGroup = new QGroupBox(tr("LED灯测试"));
//  _mainGroup->setStyleSheet("background-color:white");//因为那几张图片背景是白色的，以此达到假透明

    QVBoxLayout *_mainVLayout = new QVBoxLayout(_mainGroup);
    QGridLayout *_topGrid = new QGridLayout;
    _topGrid->setHorizontalSpacing(20);
    QStringList _bitmapNameList;
    _bitmapNameList <<"左转"<<"充电指示"<<"油压低"<<"水温高"<<"气压低"<<"助力转向"<<"倒车指示"<<"仪表电源"<<"变速箱蠕动"<<"进气预热"<<"后雾灯"<<"远光灯"<<"左右门开报警"<<"右转"
                    <<""<<""<<"水位报警"<<"驻车制动"<<"停车检查"<<"取力指示"<<"变速箱故障"<<"轮间差速"<<"燃油报警"<<"行车制动"<<"轴间差速"<<"排气制动"<<""<<""
                    <<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<"空调开启"<<"xxx"<<""
                    <<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<""<<"高低档切换"<<""<<"";

    for(int i=0; i < BITMAP_NAME_COUNT; i++)
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

            _ledStateLabels[i] = new QLabel;
            _ledStateLabels[i]->setStyleSheet("border-radius:25px;background:#fff;border:1px solid #666;");
            _ledStateLabels[i]->setFixedSize(50,50);

            _ledStateVLayout->addWidget(_ledStateBitmaps[i]);
            _ledStateVLayout->addWidget(_ledStateLabels[i]);
            _ledStateVLayout->addWidget(_ledName);
            _topGrid->addLayout(_ledStateVLayout,(int)(i/14),i - ((int)(i/14))*14);
        }
    }
    _startButton = new QPushButton(tr("开始检测"));
    _startButton->setProperty("type", "startbtn");
    _mainVLayout->addWidget(_startButton);
    _mainVLayout->addWidget(_statusLabel);
    _mainVLayout->addLayout(_topGrid);

    top->addWidget(_mainGroup);
    setLayout(top);
    //***signal
    connect(_startButton,SIGNAL(clicked()),this,SLOT(startButtonClicked()));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_led(QByteArray)),this,SLOT(sendBackData(QByteArray)));
}

void CFullTestLEDWindow::startButtonClicked()
{
    if(!property("testFlag").toInt())
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[1],0);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

         _startButton->setText(tr("停止检测"));
        emit newMessage(tr("开始LED检测"),0);
        setProperty("testFlag", 1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        clearBit(b.command[1],0);
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        newMessage(tr("结束LED检测"));
        _startButton->setText(tr("开始检测"));
        setProperty("testFlag",0);
    }
}

void CFullTestLEDWindow::sendBackData(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        int i;
        hardwareTestRespondFrame_0x20 rt = frame20FromData(a);

        _statusLabel->setText(tr("指示灯亮度：") + QString::number(rt.led_light*100) + tr("%"));

        for(i=0;i<cf->_map_led.size();i++)
        {
            int j = cf->_map_led[i];
            if(j==-1)
                continue;
            if(getTestBit(rt.led,i))
                _ledStateLabels[j]->setStyleSheet("background-color:red;border-radius:25px;border:1px solid #666;");
            else
                _ledStateLabels[j]->setStyleSheet("border-radius:25px;background:#fff;border:1px solid #666;");
        }
        }
}
