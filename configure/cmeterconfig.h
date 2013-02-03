#ifndef CMETERCONFIG_H
#define CMETERCONFIG_H

#include <QObject>
#include <QtCore>
#include "protocol/protocol.h"

class CMeterConfig : public QObject
{
    Q_OBJECT
public:
    explicit CMeterConfig(QObject *parent = 0);
    
    QMap<int,int>           _channel_type;
    QMap<int,vu16>          _channel_value;

    QMap<int,int>           _map_ioin;
    QMap<int,int>           _map_ioin_def;
    QMap<int,int>           _map_ioin_dif_neg;
    QMap<int,int>           _map_ioout;
    QMap<int,int>           _map_freq;
    QMap<int,int>           _map_current;
    QMap<int,int>           _map_resistor;
    QMap<int,int>           _map_voltage;
    QMap<int,int>           _map_power;
    QMap<int,int>           _map_cfb_voltage;
    QMap<int,int>           _map_led;
    QMap<int,int>           _map_led_io;
    int                     _map_meter_voltage;

    QMap<QString,int>       _function_param;
    QMap<QString,vu16>      _function_tpoint;

    int range_current_min;
    int range_current_max;
    int range_resistor_min;
    int range_resistor_max;
    int range_voltage_min;
    int range_voltage_max;
    int range_power_min;
    int range_power_max;

    quint32 readFile(QString fl);

signals:
    
public slots:
    
};

#endif // CMETERCONFIG_H
