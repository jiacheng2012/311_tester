#include "cfulltestioinwindow.h"
#include "protocol/protocol.h"
#include "capp.h"
#include "processing/ctestjob.h"

CFullTestIOInWindow::CFullTestIOInWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);
    setProperty("init",0);
    setProperty("in",0);

    cf = ((CApp*)qApp)->_tjob->_mconfig;

    _ioItemCount = cf->_map_ioin.size();
    if(!property("init").toInt())
    {
        setProperty("init",1);
        QVBoxLayout *top = new QVBoxLayout;

        for(int i=0; i != _ioItemCount; ++i)
        {
            QLabel *_tempLabel = new QLabel;
            _tempLabel->setStyleSheet("border-radius:20px;background:#fff;border:1px solid #666;");
            _tempLabel->setFixedSize(40,40);
            _vec32IO.push_back(_tempLabel);
        }
        _testButton = new QPushButton(tr("开始检测"));
        _statusLabel = new QLabel(tr("IO输入测试按照5秒钟间隔高低电平轮流发送。"));
        _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");
        _statusLabel_1 = new QLabel(tr("当前电平为："));
        _statusLabel_1->setStyleSheet("font:bold 18px;color:#0099FF;max-height:30px;min-height:30px;background:#CCFF99;");
        _testButton->setFocusPolicy(Qt::NoFocus);
        _bReadCurrent = new QCheckBox("读取激励电流");
        _bReadCurrent->setFocusPolicy(Qt::NoFocus);

        //***Layout
        QGroupBox *_mainGroupBox = new QGroupBox(QString::number(_ioItemCount) + tr("路IO输入量测试"));
        QGridLayout *_mainGridLay = new QGridLayout(_mainGroupBox);

        _mainGridLay->addWidget(_statusLabel,0,0,1,8);
        _mainGridLay->addWidget(_statusLabel_1,1,0,1,8);
        for(int i=0; i != _ioItemCount; ++i)
        {
            QVBoxLayout *_tempVLay = new QVBoxLayout;
            int kk = ((CApp*)qApp)->_tjob->getIOInMapVoltage(i);
            QLabel *io = new QLabel( kk?tr("高电平有效"):tr("低电平有效") );
            if(kk)
                io->setStyleSheet("background-color:green;");
            else
                io->setStyleSheet("background-color:blue;");

            _tempVLay->addWidget(new QLabel("I/O量"+QString("%1").arg(i)));
            _tempVLay->addWidget(_vec32IO.at(i));
            _tempVLay->addWidget(io);

            _mainGridLay->addLayout(_tempVLay,i/8+2,i%8,1,1);

            if( ((CApp*)qApp)->_tjob->getIOInMapCurrent(i)!=-1 )
                _vec32IO[i]->setText(tr("电流"));
        }
        _mainGridLay->addWidget(_bReadCurrent,6,4,1,2);
        _mainGridLay->addWidget(_testButton,6,6,1,2);

        top->addWidget(_mainGroupBox);
        top->setSizeConstraint(QLayout::SetFixedSize);
        setLayout(top);

        //***Signal
        connect(_testButton,SIGNAL(clicked()),this,SLOT(testButtonClicked()));
        connect((CApp*)qApp,SIGNAL(sendBackFullTestData_ioi_485(QByteArray)),this,SLOT(sendBackData485(QByteArray)));
        connect((CApp*)qApp,SIGNAL(sendBackFullTestData_ioi_232(QByteArray)),this,SLOT(sendBackData232(QByteArray)));
        _timer = new QTimer;
        connect(_timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    }
}

void CFullTestIOInWindow::timerUpdate()
{
    //***PLC
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    //***仪表
    hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
    QByteArray tmp1=QByteArray::fromRawData((const char*)&b,sizeof(b));
    ((CApp*)qApp)->_tjob->sendTestData232(tmp1);

    if(property("in").toInt())
    {
        setProperty("in",0);
        setMap(&c,0);
        c.ifeedback = _bReadCurrent->isChecked()?0xf:0x0;

        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        _statusLabel->setText(tr("请观察低电平有效的IO量是否变红，同时观察相应的激励电流(mA)。"));
        _statusLabel_1->setText(tr("当前电平：低电平"));
    }
    else
    {
        setProperty("in",1);
        setMap(&c,1);
        c.ifeedback = _bReadCurrent->isChecked()?0x1:0x0;

        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        _statusLabel->setText(tr("请观察高电平有效的IO量是否变红。"));
        _statusLabel_1->setText(tr("当前电平：高电平"));
    }
}

void CFullTestIOInWindow::testButtonClicked()
{
    int i;
    if(!property("testFlag").toBool())
    {
        _timer->start(_MS_IDLE_);
        _testButton->setText(tr("停止检测"));
        emit newMessage(tr("开始IO输入检测"),0);
        setProperty("testFlag", 1);
    }
    else
    {
        _timer->stop();

        //***信号板IO输出归位
        PLCCommandFrame_0x80 c;
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        for(int i=0; i != _ioItemCount; ++i)
        {
            if( ((CApp*)qApp)->_tjob->getIOInMapCurrent(i))
                _vec32IO[i]->setText(tr("电流"));
            _vec32IO[i]->setStyleSheet("border-radius:20px;background:#fff;border:1px solid #666;");
        }
        _testButton->setText(tr("开始检测"));
        emit newMessage(tr("IO输入检测停止"),0);
        setProperty("testFlag",0);
    }
}

void CFullTestIOInWindow::sendBackData485(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        if(_bReadCurrent->isChecked())
        {
            if(!property("in").toInt())
            {
                PLCCResponseFrame_0x81 rt = frame81FromData(a);
                for(int i=0;i<cf->_map_cfb_voltage.size();i++)
                {

                    int kk = ((CApp*)qApp)->_tjob->getIOInMapCurrentChannel(i);
                    double vald = (double)rt.vin[cf->_map_cfb_voltage[i]]/(double)((CApp*)qApp)->_config->current_fb_resistor*100.0;
                    _vec32IO[kk]->setText(QString::number((int)vald) + "mA");
                }
            }
            else
            {
                for(int i=0;i<cf->_map_cfb_voltage.size();i++)
                {
                    int kk = ((CApp*)qApp)->_tjob->getIOInMapCurrentChannel(i);
                    _vec32IO[kk]->setText(tr("电流"));
                }
            }
        }
        else
        {
            for(int i=0;i<cf->_map_cfb_voltage.size();i++)
            {
                int kk = ((CApp*)qApp)->_tjob->getIOInMapCurrentChannel(i);
                _vec32IO[kk]->setText(tr("电流"));
            }
        }
    }
}


void CFullTestIOInWindow::sendBackData232(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        int i;
        hardwareTestRespondFrame_0x20 rt = frame20FromData(a);

        for(int i=0;i<_ioItemCount;i++)
        {
            if( (getTestBit(rt.ioi,i) && ((CApp*)qApp)->_tjob->getIOInMapVoltage(i)) ||
                    (!getTestBit(rt.ioi,i) && !((CApp*)qApp)->_tjob->getIOInMapVoltage(i)) )
                _vec32IO[i]->setStyleSheet("border-radius:20px;background:red;border:1px solid #666;");
            else
                _vec32IO[i]->setStyleSheet("border-radius:20px;background:#fff;border:1px solid #666;");
        }
    }
}

void CFullTestIOInWindow::setMap(PLCCommandFrame_0x80* a,int label)
{
    for(int i=0;i<_ioItemCount;i++)
    {
        int j = cf->_map_ioin[i];
        int d = (int)(j/4);
        int r = j - d*4;

        a->ioo[d] &= ~(3<<(2*r));
        if(label==1)
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
}
