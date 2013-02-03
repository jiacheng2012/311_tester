#include <iostream>
#include <QMessageBox>
#include <cmath>
#include <QTime>

#include "capp.h"

using namespace std;

//***Constructor
CApp::CApp(int& argc, char* argv[]):
  QApplication(argc, argv)
{
    //***设置程序标识
    setApplicationName("ZZJC_311_Tester");
    setOrganizationName("ZZJC");
    setOrganizationDomain("www.zzjctech.com");
    setWindowIcon(QPixmap(":rtknav.png"));

    //***配置文件
    _config = new CConfig();
    _config->readFile(QApplication::applicationDirPath()+QString("/main.ini"));

    //***数据库
    _db = new CDatabase(_config->_dbName);

    //***
    _tjob = 0;
    _meter = 0;
    _appMemory = new QSharedMemory(tr("311仪表检测控制软件"), this);
}

//***Destructor
CApp::~CApp()
{
    if(_appMemory->isAttached())
        _appMemory->detach();
}

qint64 CApp::isRunning()
{
    //***如果已经启动
    if(_appMemory->attach(QSharedMemory::ReadOnly))
    {
        return (qint64)_appMemory->data();
    }

    if(_appMemory->create(sizeof(qint64)))
    {
        return 0;
    }

    return -1;
}

void CApp::addUser(CUser &u)
{
    _user = new CUser();
    _user->_user = u._user;
    _user->_level = u._level;
}

void CApp::addTestJobFromID(QString id)
{
    //***生成Meter类
    _meter = new CMeter(id);
    //***检查这块表是不是已经检测过
    //if(!_db->getMeter(id))
    //{
    //    _db->insertMeter(id,_user->_user,QDateTime::currentDateTime());
    //}

    //***插入一个新检测纪录
    _db->insertTest(id,_user->_user,QDateTime::currentDateTime());

    //***读出所有曾经检测记录（只检出成功的记录）
    _meter->_tr = _db->getTest(id);
    emit updateTestJobSetting();
    emit jobSetup(id,QString::number(_user->_id),_user->_name);
}

void CApp::startJob()
{
    _tjob = new CTestJob(_meter->_sid,this);
    connect(_tjob,SIGNAL(newMessage(QString,bool)),this,SIGNAL(newMessage(QString,bool)));
    connect(_tjob,SIGNAL(_232SetupFullTest()),this,SIGNAL(_232SetupFullTest()));
    connect(_tjob,SIGNAL(_232SetupAdjustTest()),this,SIGNAL(_232SetupAdjustTest()));
    connect(_tjob,SIGNAL(_232SetupFunctionTest()),this,SIGNAL(_232SetupFunctionTest()));
    connect(_tjob,SIGNAL(_485SetupFullTest()),this,SIGNAL(_485SetupFullTest()));
    connect(_tjob,SIGNAL(_485SetupAdjustTest()),this,SIGNAL(_485SetupAdjustTest()));
    connect(_tjob,SIGNAL(_485SetupFunctionTest()),this,SIGNAL(_485SetupFunctionTest()));

    connect(_tjob,SIGNAL(stop232()),this,SIGNAL(stop232()));
    connect(_tjob,SIGNAL(stop485()),this,SIGNAL(stop485()));

    connect(_tjob,SIGNAL(sendBackFullTestData_key(QByteArray)),this,SIGNAL(sendBackFullTestData_key(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_led(QByteArray)),this,SIGNAL(sendBackFullTestData_led(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_lcdbacklight(QByteArray)),this,SIGNAL(sendBackFullTestData_lcdbacklight(QByteArray)));

    connect(_tjob,SIGNAL(sendBackFullTestData_freq_232(QByteArray)),this,SIGNAL(sendBackFullTestData_freq_232(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_ioi_232(QByteArray)),this,SIGNAL(sendBackFullTestData_ioi_232(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_ioo_232(QByteArray)),this,SIGNAL(sendBackFullTestData_ioo_232(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_analog_232(QByteArray)),this,SIGNAL(sendBackFullTestData_analog_232(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_can_232(QByteArray)),this,SIGNAL(sendBackFullTestData_can_232(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_can_485(QByteArray)),this,SIGNAL(sendBackFullTestData_can_485(QByteArray)));

    connect(_tjob,SIGNAL(sendBackFullTestData_freq_485(QByteArray)),this,SIGNAL(sendBackFullTestData_freq_485(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_ioi_485(QByteArray)),this,SIGNAL(sendBackFullTestData_ioi_485(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_ioo_485(QByteArray)),this,SIGNAL(sendBackFullTestData_ioo_485(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_analog_485(QByteArray)),this,SIGNAL(sendBackFullTestData_ioi_232(QByteArray)));

    connect(_tjob,SIGNAL(sendBackAdjustTestData_speed(CDataFrame)),this,SIGNAL(sendBackAdjustTestData_speed(CDataFrame)));
    connect(_tjob,SIGNAL(sendBackAdjustTestData_rotate(CDataFrame)),this,SIGNAL(sendBackAdjustTestData_rotate(CDataFrame)));
    connect(_tjob,SIGNAL(sendBackAdjustTestData_watertemp(CDataFrame)),this,SIGNAL(sendBackAdjustTestData_watertemp(CDataFrame)));
    connect(_tjob,SIGNAL(sendBackAdjustTestData_oil(CDataFrame)),this,SIGNAL(sendBackAdjustTestData_oil(CDataFrame)));
    connect(_tjob,SIGNAL(sendBackAdjustTestData_analog_232(CDataFrame)),this,SIGNAL(sendBackAdjustTestData_analog_232(CDataFrame)));
    connect(_tjob,SIGNAL(sendBackAdjustTestData_analog_485(QByteArray)),this,SIGNAL(sendBackAdjustTestData_analog_485(QByteArray)));

    connect(_tjob,SIGNAL(sendBackFullTestData_memory(QByteArray)),this,SIGNAL(sendBackFullTestData_memory(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_selfpower(QByteArray)),this,SIGNAL(sendBackFullTestData_selfpower(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_metervoltage232(QByteArray)),this,SIGNAL(sendBackFullTestData_metervoltage232(QByteArray)));
    connect(_tjob,SIGNAL(sendBackFullTestData_metervoltage485(QByteArray)),this,SIGNAL(sendBackFullTestData_metervoltage485(QByteArray)));

    connect(_tjob,SIGNAL(sendBackFunctionTestData_232(CDataFrame)),this,SIGNAL(sendBackFunctionTestData_232(CDataFrame)));

}

void CApp::deleteJob()
{
    if(_tjob==0)
        return;

    _tjob->clearJob();
    _tjob=0;
}

void CApp::sleep(quint32 msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
