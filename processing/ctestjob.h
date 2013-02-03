#ifndef CTESTJOB_H
#define CTESTJOB_H

#include <QObject>
#include <QMap>
#include "processing/cmeter.h"
#include "processing/c232thread.h"
#include "processing/c485thread.h"
#include "protocol/protocol.h"
#include "protocol/ctestpoint.h"
#include "configure/cadjustconfig.h"
#include "configure/cmeterconfig.h"

class CTestJob : public QObject
{
    Q_OBJECT
public:
    CTestJob(QString id,QObject *parent = 0);
    ~CTestJob();

public:
    int                 _ttype,_jtype;
    int                 _msg;
    int                 _status232,_status485;

    QByteArray                  _pool232,_pool485;
    QVector<QByteArray>         _list232,_list485;
    QMap<quint8,CTestPoint>    _result_full,_result_adjust,_result_function;
    testRecord                  _tr;

    CAdjustConfig       *_aconfig;
    CMeterConfig        *_mconfig;

    int     _232tag,_485tag,tid;
    QMutex                  _mutex1,_mutex2;

protected:
    void timerEvent(QTimerEvent* event);

public:
    C232Thread          *_232t;
    C485Thread          *_485t;


    void setupSendFrame232(QByteArray,int);
    void setupSendFrame485(QByteArray,int);
    void processFrameData232(CDataFrame);
    void processFrameData485(CDataFrame);

    void sendTestData232(QByteArray);
    void sendTestData485(QByteArray);

    void sendBackTestData232(CDataFrame);
    void sendBackTestData485(CDataFrame);
    void sendBackFunctionData232(CDataFrame a);
    void sendBackAdjustData232(CDataFrame);
    void sendBackAdjustData485(CDataFrame);

    void connect232(int);
    void connect485(int);
    void delete232();
    void delete485();
    void start232T(int);
    void stop232T();
    void start485T(int);
    void stop485T();
    void clearJob();
    void initFrame80(PLCCommandFrame_0x80* a);
    bool getIOInMapVoltage(quint8 index);
    int  getIOInMapCurrent(quint8 index);
    int  getIOInMapCurrentChannel(int index);
    int  getIOInMapDifference(quint8 index);

signals:
    void newMessage(QString,bool);
    void _232SetupFullTest();
    void _232SetupAdjustTest();
    void _232SetupFunctionTest();
    void _485SetupFullTest();
    void _485SetupAdjustTest();
    void _485SetupFunctionTest();
    void stop232();
    void stop485();

    //***
    void sendBackFullTestData_key(QByteArray);
    void sendBackFullTestData_led(QByteArray);
    void sendBackAdjustTestData_speed(CDataFrame);
    void sendBackFullTestData_freq_232(QByteArray);
    void sendBackFullTestData_ioi_232(QByteArray);
    void sendBackFullTestData_ioo_232(QByteArray);
    void sendBackFullTestData_analog_232(QByteArray);
    void sendBackFullTestData_can_232(QByteArray);
    void sendBackFullTestData_lcdbacklight(QByteArray);

    void sendBackFullTestData_freq_485(QByteArray);
    void sendBackFullTestData_ioi_485(QByteArray);
    void sendBackFullTestData_ioo_485(QByteArray);
    void sendBackFullTestData_analog_485(QByteArray);
    void sendBackFullTestData_can_485(QByteArray);

    void sendBackAdjustTestData_watertemp(CDataFrame);
    void sendBackAdjustTestData_rotate(CDataFrame);
    void sendBackAdjustTestData_oil(CDataFrame);
    void sendBackAdjustTestData_analog_232(CDataFrame);
    void sendBackAdjustTestData_analog_485(QByteArray);
    void sendBackFullTestData_memory(QByteArray);
    void sendBackFullTestData_selfpower(QByteArray);
    void sendBackFullTestData_metervoltage232(QByteArray);
    void sendBackFullTestData_metervoltage485(QByteArray);

    void sendBackFunctionTestData_232(CDataFrame);

public slots:
    void get232Data(QByteArray);
    void get485Data(QByteArray);
    void delaySetupAdjustTest();
    void delayClearJob();
};

#endif
