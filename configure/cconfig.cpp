#include <QtCore>
#include "cconfig.h"
#include "protocol/protocol.h"

CConfig::CConfig(QObject *parent) :
    QObject(parent)
{
}

quint32 CConfig::readFile(QString f1)
{
    QSettings *sIniSetting = new QSettings(f1, QSettings::IniFormat);
    sIniSetting->setIniCodec("UTF-8");
    //***Reading all settings here
    _dbName = sIniSetting->value("database/dbname").toString();

    //***读取PLC,仪表板,设置
    _connectParmsMap.clear();
    _connectParmsMap.insert("plc-portnumber",sIniSetting->value("plcset/portnumber").toInt());
    _connectParmsMap.insert("plc-baudrate",sIniSetting->value("plcset/baudrate").toInt());
    _connectParmsMap.insert("plc-databit",sIniSetting->value("plcset/databit").toInt());
    _connectParmsMap.insert("plc-paritycheck",sIniSetting->value("plcset/paritycheck").toInt());
    _connectParmsMap.insert("plc-stopbit",sIniSetting->value("plcset/stopbit").toInt());
    _connectParmsMap.insert("plc-dataflowcontrol",sIniSetting->value("plcset/dataflowcontrol").toInt());

    _connectParmsMap.insert("meter-portnumber",sIniSetting->value("meterset/portnumber").toInt());
    _connectParmsMap.insert("meter-baudrate",sIniSetting->value("meterset/baudrate").toInt());
    _connectParmsMap.insert("meter-databit",sIniSetting->value("meterset/databit").toInt());
    _connectParmsMap.insert("meter-paritycheck",sIniSetting->value("meterset/paritycheck").toInt());
    _connectParmsMap.insert("meter-stopbit",sIniSetting->value("meterset/stopbit").toInt());
    _connectParmsMap.insert("meter-dataflowcontrol",sIniSetting->value("meterset/dataflowcontrol").toInt());

    _mfile = sIniSetting->value("config_file/meterfile").toString();
    _afile = sIniSetting->value("config_file/adjustfile").toString();

    current_fb_resistor = sIniSetting->value("hardware/current_fb_resistor").toInt();
    load0 = sIniSetting->value("hardware/load0").toInt();
    load1 = sIniSetting->value("hardware/load1").toInt();
    load2 = sIniSetting->value("hardware/load2").toInt();
    load3 = sIniSetting->value("hardware/load3").toInt();
    load4 = sIniSetting->value("hardware/load4").toInt();
    load5 = sIniSetting->value("hardware/load5").toInt();

    if(_dbName!="")
        return 1;
    else
        return 0;
}
