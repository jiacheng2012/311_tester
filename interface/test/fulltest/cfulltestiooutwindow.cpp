#include "cfulltestiooutwindow.h"
#include "protocol/protocol.h"
#include "capp.h"

CFullTestIOOutWindow::CFullTestIOOutWindow(QWidget *parent) :
    QWidget(parent)
{
    setProperty("testFlag",0);

    //***Signal
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_ioo_485(QByteArray)),this,SLOT(sendBackData485(QByteArray)));
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_ioo_232(QByteArray)),this,SLOT(sendBackData232(QByteArray)));

    cf = ((CApp*)qApp)->_tjob->_mconfig;


        //***UI components
        QVBoxLayout *top = new QVBoxLayout;
        _statusLabel = new QLabel(tr("IO输出测试按照2秒钟间隔轮流通道发送。"));
        _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

        _itemCount = cf->_map_ioout.size();
        for(int i=0; i != _itemCount; ++i)
        {
            QLabel *_tempLabel1 = new QLabel;
            _tempLabel1->setStyleSheet("border-radius:18px;background:#fff;border:1px solid #666;");
            _tempLabel1->setFixedSize(36,36);
            QLabel *_tempLabel2 = new QLabel;
            _tempLabel2->setStyleSheet("border-radius:18px;background:#fff;border:1px solid #666;");
            _tempLabel2->setFixedSize(36,36);
            _dashboardMark.push_back(_tempLabel1);
            _plcMark.push_back(_tempLabel2);
        }
        _testButton=new QPushButton(tr("I/O量输出测试"));

        //***Layout
        QGroupBox *_mainGroupBox = new QGroupBox(QString::number(_itemCount) + tr("路I/O量输出测试"));
        QGridLayout *_mainGridLayout = new QGridLayout(_mainGroupBox);
        _mainGridLayout->addWidget(_statusLabel,0,0,1,_itemCount+1);
        _mainGridLayout->addWidget(new QLabel(tr("仪表板输出：")),2,0);
        _mainGridLayout->addWidget(new QLabel(tr("信号箱采样：")),3,0);

        for(int i=0;i<_itemCount;i++)
        {
            QLabel *tempLabel = new QLabel(tr("I/O量")+QString::number(i,10));
            tempLabel->setAlignment(Qt::AlignCenter);
            _mainGridLayout->addWidget(tempLabel,1,i+1);
            _mainGridLayout->addWidget(_dashboardMark.at(i),2,i+1);
            _mainGridLayout->addWidget(_plcMark.at(i),3,i+1);
        }
        _mainGridLayout->addWidget(_testButton,4,7,1,2);

    top->addWidget(_mainGroupBox);
    top->setSizeConstraint(QLayout::SetFixedSize);
    top->setAlignment(Qt::AlignCenter);
    setLayout(top);

    connect(_testButton,SIGNAL(clicked()),this,SLOT(testButtonClicked()));

}

void CFullTestIOOutWindow::testButtonClicked()
{
    if(!property("testFlag").toBool())
    {
        //***仪表
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setBit(b.command[1],3);
        QByteArray tmp1=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp1);

        //***PLC
        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

         _testButton->setText(tr("停止检测"));
        emit newMessage(tr("开始IO输入检测"),0);
        setProperty("testFlag", 1);
    }
    else
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        QByteArray tmp1=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp1);

        PLCCommandFrame_0x80 c={0};
        ((CApp*)qApp)->_tjob->initFrame80(&c);
        setMap(&c,0);
        QByteArray tmp2=QByteArray::fromRawData((const char*)&c,sizeof(c));
        ((CApp*)qApp)->_tjob->sendTestData485(tmp2);

        _testButton->setText(tr("开始检测"));
        emit newMessage(tr("IO输入检测停止"),0);
        setProperty("testFlag",0);
    }
}


void CFullTestIOOutWindow::sendBackData485(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        PLCCResponseFrame_0x81 rt = frame81FromData(a);

        for(int i=0;i<_itemCount;i++)
        {
            if(getMap(rt,i))
                _plcMark[i]->setStyleSheet("border-radius:18px;background:red;border:1px solid #666;");
            else
                _plcMark[i]->setStyleSheet("border-radius:18px;background:#fff;border:1px solid #666;");
        }
    }
}

void CFullTestIOOutWindow::sendBackData232(QByteArray a)
{
    if(property("testFlag").toBool())
    {
        hardwareTestRespondFrame_0x20 rt = frame20FromData(a);

        for(int i=0;i<_itemCount;i++)
        {
            if(getTestBit(rt.ioo,i))
                _dashboardMark[i]->setStyleSheet("border-radius:18px;background:red;border:1px solid #666;");
            else
                _dashboardMark[i]->setStyleSheet("border-radius:18px;background:#fff;border:1px solid #666;");
        }
    }
}

void CFullTestIOOutWindow::setMap(PLCCommandFrame_0x80* a,int label)
{
    for(int i=0;i<8;i++)
    {
        int j = cf->_map_ioout[i];
        if(j<8)
        {
            if(label==1)
                a->ioi[0] |= 1<<j;
            else
                a->ioi[0] &= (~(1<<j));
        }
        else
        {
            if(label==1)
                a->ioi[1] |= 1<<(j-8);
            else
                a->ioi[1] &= (~(1<<(j-8)));
        }
    }
}

bool CFullTestIOOutWindow::getMap(PLCCResponseFrame_0x81 a,int i)
{
    int j = cf->_map_ioout[i];

    if(j<8)
    {
        return getTestBit(a.ioi[0],j);
    }
    else if(j<16)
    {
        return getTestBit(a.ioi[1],j-8);
    }
    else
    {
        return getTestBit(a.ioi[2],j-16);
    }
}
