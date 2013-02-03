#include "cadjusttestoverviewwindow.h"
#include "processing/ctestjob.h"
#include "capp.h"

CAdjustTestOverviewWindow::CAdjustTestOverviewWindow(QWidget *parent) :
    QWidget(parent)
{
    _connected = 0;

    QHBoxLayout* topHBoxLayout = new QHBoxLayout;
    QLabel* tip1 = new QLabel(tr("点击建立仪表串口按钮以后请重新启动仪表"));
    tip1->setStyleSheet("font:bold 14px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;padding:0 15px;");
    tip1->setAlignment(Qt::AlignCenter);
    QGroupBox* mainGroup = new QGroupBox(tr("连接设置"));
    QVBoxLayout* mainVLayout = new QVBoxLayout(mainGroup);
    _485Button = new QPushButton(tr("建立信号箱串口连接"));
    _232Button = new QPushButton(tr("建立仪表串口连接"));
    _nextButton = new QPushButton(tr("下一步"));
    _nextButton->setEnabled(false);
    _232Button->setEnabled(false);

    _232Button->setProperty("page","overview");
    _485Button->setProperty("page","overview");
    _nextButton->setProperty("page","overview");

    QGroupBox* shortcuts = new QGroupBox(tr("仪表调校通用快捷键"));
    QGridLayout* shortcutsGrid = new QGridLayout(shortcuts);
    QLabel*  pointertozero = new QLabel(tr("指针归零<br> (Z)"));
    pointertozero->setAlignment(Qt::AlignCenter);
    pointertozero->setStyleSheet("font:bold 14px;color:white;max-height:50px;min-height:50px;background:#2FA3FE;border-radius:12px;padding:0 5px;");
    QLabel*  movenext = new QLabel(tr("调校项后移<br>(A)"));
    movenext->setAlignment(Qt::AlignCenter);
    movenext->setStyleSheet("font:bold 14px;color:white;max-height:50px;min-height:50px;background:#2FA3FE;border-radius:12px;padding:0 5px;");
    QLabel*  movepre = new QLabel(tr("调校项前移<br>(D)"));
    movepre->setAlignment(Qt::AlignCenter);
    movepre->setStyleSheet("font:bold 14px;color:white;max-height:50px;min-height:50px;background:#2FA3FE;border-radius:12px;padding:0 5px;");
    QLabel*  stepup = new QLabel(tr("预制置步数增大<br>(W)"));
    stepup->setAlignment(Qt::AlignCenter);
    stepup->setStyleSheet("font:bold 14px;color:white;max-height:50px;min-height:50px;background:#2FA3FE;border-radius:12px;padding:0 5px;");
    QLabel*  stepdown = new QLabel(tr("预置微步数减小<br>(S)"));
    stepdown->setAlignment(Qt::AlignCenter);
    stepdown->setStyleSheet("font:bold 14px;color:white;max-height:50px;min-height:50px;background:#2FA3FE;border-radius:12px;padding:0 5px;");
    QLabel*  readparm = new QLabel(tr("读取调校参数<br>(R)"));
    readparm->setAlignment(Qt::AlignCenter);
    readparm->setStyleSheet("font:bold 14px;color:white;max-height:50px;min-height:50px;background:#2FA3FE;border-radius:12px;padding:0 5px;");
    QLabel*  saveparm = new QLabel(tr("保存调校参数<br>(CTRL + S)"));
    saveparm->setAlignment(Qt::AlignCenter);
    saveparm->setStyleSheet("font:bold 14px;color:white;max-height:50px;min-height:50px;background:#2FA3FE;border-radius:12px;padding:0 5px;");
    shortcutsGrid->addWidget(movenext,0,0);
    shortcutsGrid->addWidget(movepre,0,1);
    shortcutsGrid->addWidget(stepup,1,0);
    shortcutsGrid->addWidget(stepdown,1,1);
    shortcutsGrid->addWidget(pointertozero,2,0);
    shortcutsGrid->addWidget(readparm,3,0);
    shortcutsGrid->addWidget(saveparm,3,1);
    shortcutsGrid->setContentsMargins(15,10,15,10);
    shortcutsGrid->setSpacing(6);

    mainVLayout->addWidget(tip1,0,Qt::AlignCenter);
    mainVLayout->addWidget(_485Button,0,Qt::AlignCenter);
    mainVLayout->addWidget(_232Button,0,Qt::AlignCenter);
    mainVLayout->addWidget(_nextButton,0,Qt::AlignCenter);
    mainVLayout->setContentsMargins(30,20,30,20);
    mainVLayout->setSpacing(20);

    topHBoxLayout->addWidget(mainGroup);
    topHBoxLayout->addWidget(shortcuts);

    setLayout(topHBoxLayout);

    //***
    connect(_232Button,SIGNAL(clicked()),this,SLOT(_232ButtonClicked()));
    connect(_485Button,SIGNAL(clicked()),this,SLOT(_485ButtonClicked()));
    connect(_nextButton,SIGNAL(clicked()),this,SLOT(nextButtonClicked()));
    connect((CApp*)qApp,SIGNAL(_232SetupAdjustTest()),this,SLOT(_232Setup()));
    connect((CApp*)qApp,SIGNAL(_485SetupAdjustTest()),this,SLOT(_485Setup()));
    connect((CApp*)qApp,SIGNAL(stop232()),this,SLOT(stop232()));
    connect((CApp*)qApp,SIGNAL(stop485()),this,SLOT(stop485()));
}



void CAdjustTestOverviewWindow::_232ButtonClicked()
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
            job->connect232(ADJUSTTEST_ID);
        }
    }
    else
        emit newMessage(tr("没有进行作业设置，请先设置"),1);
}

void CAdjustTestOverviewWindow::_485ButtonClicked()
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
            job->connect485(ADJUSTTEST_ID);
        }
    }
    else
        emit newMessage(tr("没有进行作业设置，请先设置"),1);
}

void CAdjustTestOverviewWindow::_232Setup()
{
    _232Button->setEnabled(true);
    _connected |= 0x1;
    if(_connected&1 && _connected&2)
        _nextButton->setEnabled(true);
    _232Button->setText(tr("建立仪表串口连接"));
    emit newMessage(tr("仪表已经设置为硬件调校模式"),0);
}

void CAdjustTestOverviewWindow::_485Setup()
{
    _485Button->setEnabled(true);
    _232Button->setEnabled(true);
    _connected |= 0x2;
    if(_connected&1 && _connected&2)
        _nextButton->setEnabled(true);
    _485Button->setText(tr("建立信号箱串口连接"));
}

void CAdjustTestOverviewWindow::stop232()
{
    _232Button->setEnabled(true);
    _connected &= ~0x2;
    _nextButton->setEnabled(false);
    _485Button->setText(tr("建立仪表串口连接"));
}

void CAdjustTestOverviewWindow::stop485()
{
    _485Button->setEnabled(true);
    _connected &= ~0x1;
    _nextButton->setEnabled(false);
    _485Button->setText(tr("建立信号箱串口连接"));
}

void CAdjustTestOverviewWindow::nextButtonClicked()
{
    emit nextList(-1);
}
