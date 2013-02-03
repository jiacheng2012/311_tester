#include "cfulltestfreqwindow.h"
#include "protocol/protocol.h"
#include "capp.h"

CFullTestFreqWindow::CFullTestFreqWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);
    //***ui component
    QVBoxLayout *top = new QVBoxLayout;
    _statusLabel = new QLabel(tr("频率量的单位是HZ，请观察采样频率和信号箱发出的频率是否相符。"));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    _standFreqVal = new QSpinBox();
    _standFreqVal->setValue(100);
    _standFreqVal->setRange(1,65535);
    _channelSampleVal0 = new QLabel();
    _channelSampleVal0->setStyleSheet("font:bold 18px;color:green;max-height:30px;min-height:30px;background:#CCFF99;");
    _channelSampleVal1= new QLabel();
    _channelSampleVal1->setStyleSheet("font:bold 18px;color:green;max-height:30px;min-height:30px;background:#CCFF99;");
    _testButton = new QPushButton(tr("频率量测试"));

    //***layout
    QGroupBox *_mainGroup = new QGroupBox(tr("频率量测试"));
    QGridLayout *_mainGrid =  new QGridLayout(_mainGroup);
    _mainGrid->addWidget(_statusLabel,0,0,1,3);
    _mainGrid->addWidget(new QLabel(tr("标准频率值：")),1,0);
    _mainGrid->addWidget(_standFreqVal,1,1);
    _mainGrid->addWidget(new QLabel(tr("Hz")),1,2);

    _mainGrid->addWidget(new QLabel(tr("频道通道0采样值：")),2,0);
    _mainGrid->addWidget(_channelSampleVal0,2,1);

    _mainGrid->addWidget(new QLabel(tr("频道通道1采样值：")),3,0);
    _mainGrid->addWidget(_channelSampleVal1,3,1);

    _mainGrid->addWidget(_testButton,4,1);

    top->addWidget(_mainGroup,0, Qt::AlignLeft | Qt::AlignTop);
    setLayout(top);

    connect(_testButton,SIGNAL(clicked()),this,SLOT(testButtonClicked()));
    connect(_standFreqVal,SIGNAL(valueChanged(int)),this,SLOT(freqValueChanged(int)));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_freq_485(QByteArray)),this,SLOT(sendBackData485(QByteArray)));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_freq_232(QByteArray)),this,SLOT(sendBackData232(QByteArray)));

    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFullTestFreqWindow::freqValueChanged(int val)
{
    if(property("testFlag").toBool())
    {
        //***PLC
        int val = _standFreqVal->value();
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        setMap(&c,val);
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);
    }
}

void CFullTestFreqWindow::testButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        //***仪表
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        QByteArray tmp1=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp1);

        //***PLC
        int val = _standFreqVal->value();
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        setMap(&c,val);
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

         _testButton->setText(tr("停止检测"));
        emit newMessage(tr("开始频率量检测"),0);
        setProperty("testFlag", 1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _testButton->setText(tr("开始检测"));
        setProperty("testFlag",0);
    }
}


void CFullTestFreqWindow::sendBackData485(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        PLCCResponseFrame_0x81 rt = frame81FromData(a);
    }
}


void CFullTestFreqWindow::sendBackData232(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        hardwareTestRespondFrame_0x20 rt = frame20FromData(a);

        _channelSampleVal0->setText(QString::number(rt.fq[0]*0.1));
        _channelSampleVal1->setText(QString::number(rt.fq[1]*0.1));
    }
}

void CFullTestFreqWindow::setMap(PLCCommandFrame_0x80* a,int val)
{
    for(int i=0;i<2;i++)
    {
        int j = cf->_map_freq[i];

        QByteArray tmp = arrangeByte(val,2);
        a->freqout[j*2] = tmp[0];
        a->freqout[j*2+1] = tmp[1];
    }
}
