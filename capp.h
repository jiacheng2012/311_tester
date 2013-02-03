#ifndef CAPP_H
#define CAPP_H

#include <QtGui>
#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QTextStream>
#include <QtDebug>
#include <QVariant>

#include "configure/cconfig.h"
#include "database/cdatabase.h"
#include "database/cuser.h"
#include "interface/cmainwindow.h"
#include "processing/cmeter.h"
#include "processing/cconfigjob.h"

class CApp : public QApplication
{
  Q_OBJECT

public:
    CApp(int& argc, char* argv[]);
    ~CApp();
    void addTestJobFromID(QString);
    void sleep(quint32 msec);
    void startJob();
    void deleteJob();
    qint64 isRunning();
public:
    CConfig             *_config;
    CDatabase           *_db;
    CUser               *_user;
    CTestJob            *_tjob;
    CMeter              *_meter;
    QSharedMemory       *_appMemory;
signals:
    void                updateTestJobSetting();
    void                newMessage(QString,bool);
    void                _232SetupFullTest();
    void                _232SetupAdjustTest();
    void                _232SetupFunctionTest();
    void                _485SetupFullTest();
    void                _485SetupAdjustTest();
    void                _485SetupFunctionTest();
    void                databaseSetup();
    void                jobSetup(QString,QString,QString);
    void                stop232();
    void                stop485();

    void                sendBackFullTestData_key(QByteArray);
    void                sendBackFullTestData_led(QByteArray);
    void                sendBackAdjustTestData_speed(CDataFrame);
    void                sendBackFullTestData_freq_232(QByteArray);
    void                sendBackFullTestData_ioi_232(QByteArray);
    void                sendBackFullTestData_ioo_232(QByteArray);
    void                sendBackFullTestData_analog_232(QByteArray);
    void                sendBackFullTestData_lcdbacklight(QByteArray);
    void                sendBackFullTestData_can_232(QByteArray);

    void                sendBackFullTestData_freq_485(QByteArray);
    void                sendBackFullTestData_ioi_485(QByteArray);
    void                sendBackFullTestData_ioo_485(QByteArray);
    void                sendBackFullTestData_analog_485(QByteArray);
    void                sendBackFullTestData_can_485(QByteArray);

    void                sendBackAdjustTestData_watertemp(CDataFrame);
    void                sendBackAdjustTestData_oil(CDataFrame);
    void                sendBackAdjustTestData_rotate(CDataFrame);

    void                sendBackFullTestData_memory(QByteArray);
    void                sendBackFullTestData_selfpower(QByteArray);
    void                sendBackFullTestData_metervoltage232(QByteArray);
    void                sendBackFullTestData_metervoltage485(QByteArray);

    void                sendBackAdjustTestData_analog_485(QByteArray);
    void                sendBackAdjustTestData_analog_232(CDataFrame);

    void                sendBackFunctionTestData_232(CDataFrame);

    void                toTestTabPage(int);

private:
    QMutex              _mutex;

public slots:
    void                addUser(CUser&);
};

#endif // APP_H
