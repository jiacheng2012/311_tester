#include <QtCore>
#include "cadjustconfig.h"

CAdjustConfig::CAdjustConfig(QObject *parent) :
    QObject(parent)
{
}

quint32 CAdjustConfig::readFile(QString f1)
{
    QSettings *sIniSetting = new QSettings(f1, QSettings::IniFormat);
    sIniSetting->setIniCodec("UTF-8");

    //***Reading all settings here
    //***setting
    date        = sIniSetting->value("setting/date").toInt();
    version     = sIniSetting->value("setting/version").toInt();
    version1    = sIniSetting->value("setting/version1").toInt();

    //***Type
    QStringList s;
    int i,j;
    s = sIniSetting->value("anatype/anatype").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        type_ana[i] = s[i].toInt();

    //***Type
    int num = type_ana.size();

    //***value
    s = sIniSetting->value("value/value_oil").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        value_meter["oil"][i] = s[i].toInt();
    s = sIniSetting->value("value/value_rspeed").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        value_meter["rspeed"][i] = s[i].toInt();
    s = sIniSetting->value("value/value_wtemp").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        value_meter["wtemp"][i] = s[i].toInt();
    s = sIniSetting->value("value/value_speed").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        value_meter["speed"][i] = s[i].toInt();

    for(i=0;i<num;i++)
    {
        s = sIniSetting->value("value/value_ana_"+QString::number(i)).toString().split(" ",QString::SkipEmptyParts);
        for(j=0;j<s.size();j++)
            value_ana[i][j] = s[j].toInt();
    }

    //***sample
    s = sIniSetting->value("sample/sample_oil").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        sample_meter["oil"][i] = s[i].toInt();
    s = sIniSetting->value("sample/sample_rspeed").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        sample_meter["rspeed"][i] = s[i].toInt();
    s = sIniSetting->value("sample/sample_wtemp").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        sample_meter["wtemp"][i] = s[i].toInt();
    s = sIniSetting->value("sample/sample_speed").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        sample_meter["speed"][i] = s[i].toInt();

    for(i=0;i<num;i++)
    {
        s = sIniSetting->value("sample/sample_ana_"+QString::number(i)).toString().split(" ",QString::SkipEmptyParts);
        for(j=0;j<s.size();j++)
            sample_ana[i][j] = s[j].toInt();
    }

    //***other
    oil_rcof = sIniSetting->value("others/oil_rcof").toInt();

    if(oil_rcof!=0)
        return 1;
    else
        return 0;

}

quint32 CAdjustConfig::writeFile(QString f1)
{
    int i;
    QSettings *sIniSetting = new QSettings(f1, QSettings::IniFormat);
    sIniSetting->setIniCodec("UTF-8");

    //***Writing all settings here
    //***setting
    sIniSetting->setValue("setting/date",QString::number(date));
    sIniSetting->setValue("setting/version",QString::number(version));
    sIniSetting->setValue("setting/version1",QString::number(version1));

    //***Type
    sIniSetting->setValue("anatype/anatype",mapToString(type_ana));

    //***value
    sIniSetting->setValue("value/value_oil",mapToString(value_meter["oil"]));
    sIniSetting->setValue("value/value_rspeed",mapToString(value_meter["rspeed"]));
    sIniSetting->setValue("value/value_wtemp",mapToString(value_meter["wtemp"]));
    sIniSetting->setValue("value/value_speed",mapToString(value_meter["speed"]));

    int num = type_ana.size();

    for(i=0;i<num;i++)
        sIniSetting->setValue("value/value_ana_"+QString::number(i),mapToString(value_ana[i]));

    //***sample
    sIniSetting->setValue("sample/sample_oil",mapToString(sample_meter["oil"]));
    sIniSetting->setValue("sample/sample_rspeed",mapToString(sample_meter["rspeed"]));
    sIniSetting->setValue("sample/sample_wtemp",mapToString(sample_meter["wtemp"]));
    sIniSetting->setValue("sample/sample_speed",mapToString(sample_meter["speed"]));

    for(i=0;i<num;i++)
        sIniSetting->setValue("sample/sample_ana_"+QString::number(i),mapToString(sample_ana[i]));

    //***other
    sIniSetting->setValue("others/oil_rcof",QString::number(oil_rcof));

    return 1;
}
