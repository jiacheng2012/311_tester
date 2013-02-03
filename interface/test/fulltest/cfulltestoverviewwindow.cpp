#include "cfulltestoverviewwindow.h"
#include "processing/ctestjob.h"
#include "capp.h"

CFullTestOverviewWindow::CFullTestOverviewWindow(QWidget *parent) :
    QWidget(parent)
{
    _connected = 0;
    QVBoxLayout* top = new QVBoxLayout;

    QLabel* tip = new QLabel(tr("点击建立仪表串口按钮以后请重新启动仪表"));
    tip->setStyleSheet("font:bold 14px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;padding:0 15px;");
    tip->setAlignment(Qt::AlignCenter);

    QGroupBox* connectGroup = new QGroupBox(tr("连接设置"));
    QVBoxLayout* connectGroupVLayout = new QVBoxLayout(connectGroup);
    _232Button = new QPushButton(tr("建立仪表串口连接"));
    _485Button = new QPushButton(tr("建立信号箱串口连接"));
    _nextButton = new QPushButton(tr("下一步"));
    _nextButton->setEnabled(false);

    _232Button->setProperty("page","overview");
    _485Button->setProperty("page","overview");
    _nextButton->setProperty("page","overview");

    connectGroupVLayout->addWidget(tip,0,Qt::AlignCenter);
    connectGroupVLayout->addWidget(_485Button,0,Qt::AlignCenter);
    connectGroupVLayout->addWidget(_232Button,0,Qt::AlignCenter);
    connectGroupVLayout->addWidget(_nextButton,0,Qt::AlignCenter);
    connectGroupVLayout->setContentsMargins(30,20,30,20);
    connectGroupVLayout->setSpacing(20);

    _232Button->setEnabled(false);

    top->addWidget(connectGroup, 0, Qt::AlignLeft | Qt::AlignTop);
    setLayout(top);
    //***
    connect(_232Button,SIGNAL(clicked()),this,SLOT(_232ButtonClicked()));
    connect(_485Button,SIGNAL(clicked()),this,SLOT(_485ButtonClicked()));
    connect(_nextButton,SIGNAL(clicked()),this,SLOT(nextButtonClicked()));
    connect((CApp*)qApp,SIGNAL(_232SetupFullTest()),this,SLOT(_232Setup()));
    connect((CApp*)qApp,SIGNAL(_485SetupFullTest()),this,SLOT(_485Setup()));
    connect((CApp*)qApp,SIGNAL(stop232()),this,SLOT(stop232()));
    connect((CApp*)qApp,SIGNAL(stop485()),this,SLOT(stop485()));
}

void CFullTestOverviewWindow::_232ButtonClicked()
{
    CTestJob* job = ((CApp*)qApp)->_tjob;
    if(job!=0)
    {
        if(job->_status232)
            emit newMessage(tr("仪表已经连接！"),1);
        else
        {
            _232Button->setText(tr("等待仪表反馈帧..."));
            _232Button->setEnabled(false);
            job->connect232(FULLTEST_ID);
        }
    }
    else
        emit newMessage(tr("没有进行作业设置，请先设置"),1);
}

void CFullTestOverviewWindow::_485ButtonClicked()
{
    CTestJob* job = ((CApp*)qApp)->_tjob;
    if(job!=0)
    {
        if(job->_status485)
            emit newMessage(tr("信号箱已经连接！"),1);
        else
        {
            _485Button->setText(tr("等待信号箱反馈帧..."));
            _485Button->setEnabled(false);
            job->connect485(FULLTEST_ID);
        }
    }
    else
        emit newMessage(tr("没有进行作业设置，请先设置"),1);
}

void CFullTestOverviewWindow::_232Setup()
{
    _232Button->setEnabled(true);
    _connected |= 0x1;
    if(_connected&1 && _connected&2)
        _nextButton->setEnabled(true);
    _232Button->setText(tr("建立仪表串口连接"));
    emit newMessage(tr("仪表已经设置为硬件全检模式"),0);
}

void CFullTestOverviewWindow::_485Setup()
{
    _485Button->setEnabled(true);
    _232Button->setEnabled(true);
    _connected |= 0x2;
    if(_connected&1 && _connected&2)
        _nextButton->setEnabled(true);
    _485Button->setText(tr("建立信号箱串口连接"));
    emit newMessage(tr("信号箱连接成功"),0);
}

void CFullTestOverviewWindow::stop232()
{
    _232Button->setEnabled(true);
    _connected &= ~0x2;
    _nextButton->setEnabled(false);
    _485Button->setText(tr("建立仪表串口连接"));
}

void CFullTestOverviewWindow::stop485()
{
    _485Button->setEnabled(true);
    _connected &= ~0x1;
    _nextButton->setEnabled(false);
    _485Button->setText(tr("建立信号箱串口连接"));
}

void CFullTestOverviewWindow::nextButtonClicked()
{
    emit nextList(-1);
}
