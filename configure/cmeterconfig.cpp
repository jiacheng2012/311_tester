#include "cmeterconfig.h"
#include <QtCore>
#include "protocol/protocol.h"

CMeterConfig::CMeterConfig(QObject *parent) :
    QObject(parent)
{
}

quint32 CMeterConfig::readFile(QString f1)
{
    QSettings *sIniSetting = new QSettings(f1, QSettings::IniFormat);
    sIniSetting->setIniCodec("UTF-8");

    //***读取IO Map
    QStringList s;
    int i,j;
    s = sIniSetting->value("iomap/map_ioin").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_ioin[i] = s[i].toInt();

    s = sIniSetting->value("iomap/map_ioin_def").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_ioin_def[i] = s[i].toInt();

    s = sIniSetting->value("iomap/map_ioin_dif_neg").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_ioin_dif_neg[i] = s[i].toInt();

    s = sIniSetting->value("iomap/map_ioout").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_ioout[i] = s[i].toInt();

    s = sIniSetting->value("iomap/map_freq").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_freq[i] = s[i].toInt();

    s = sIniSetting->value("iomap/map_current").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_current[i] = s[i].toInt();

    s = sIniSetting->value("iomap/map_resistor").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_resistor[i] = s[i].toInt();

    s = sIniSetting->value("iomap/map_voltage").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_voltage[i] = s[i].toInt();

    s = sIniSetting->value("iomap/map_power").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_power[i] = s[i].toInt();

    s = sIniSetting->value("iomap/map_cfb_voltage").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_cfb_voltage[i] = s[i].toInt();

    _map_meter_voltage = sIniSetting->value("iomap/map_meter_voltage").toInt();

    s = sIniSetting->value("iomap/map_led").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_led[i] = s[i].toInt();

    s = sIniSetting->value("iomap/map_led_io").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _map_led_io[i] = s[i].toInt();

    //***读取模拟量设置
    //***Type
    s = sIniSetting->value("analog_channel/channel_type").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _channel_type[i] = s[i].toInt();

    int num = _channel_type.size();

    for(i=0;i<num;i++)
    {
        s = sIniSetting->value("analog_channel/channel_value_"+QString::number(i)).toString().split(" ",QString::SkipEmptyParts);
        for(j=0;j<s.size();j++)
            _channel_value[i][j] = s[j].toInt();
    }

    //***仪表功能参数(对应仪表存储帧)
    _function_param.clear();
    _function_param["speed_pulse"] = sIniSetting->value("function_param/speed_pulse").toInt();
    _function_param["w_cwater"] = sIniSetting->value("function_param/w_cwater").toInt();
    _function_param["w_lpressure"] = sIniSetting->value("function_param/w_lpressure").toInt();
    _function_param["w_opressure"] = sIniSetting->value("function_param/w_opressure").toInt();
    _function_param["w_pressurel"] = sIniSetting->value("function_param/w_pressurel").toInt();
    _function_param["backlight"] = sIniSetting->value("function_param/backlight").toInt();
    _function_param["ledlight"] = sIniSetting->value("function_param/ledlight").toInt();
    _function_param["fan_lspeed_temp"] = sIniSetting->value("function_param/fan_lspeed_temp").toInt();
    _function_param["fan_hspeed_temp"] = sIniSetting->value("function_param/fan_hspeed_temp").toInt();
    _function_param["fan_lspeed_wtemp"] = sIniSetting->value("function_param/fan_lspeed_wtemp").toInt();
    _function_param["fan_hspeed_wtemp"] = sIniSetting->value("function_param/fan_hspeed_wtemp").toInt();
    _function_param["fan_lspeed_diff"] = sIniSetting->value("function_param/fan_lspeed_diff").toInt();
    _function_param["fan_hspeed_diff"] = sIniSetting->value("function_param/fan_hspeed_diff").toInt();
    _function_param["param_setting"] = sIniSetting->value("function_param/param_setting").toInt();
    _function_param["language"] = sIniSetting->value("function_param/language").toInt();
    _function_param["loil_type"] = sIniSetting->value("function_param/loil_type").toInt();
    _function_param["brake_type"] = sIniSetting->value("function_param/brake_type").toInt();
    _function_param["brake_valid"] = sIniSetting->value("function_param/brake_valid").toInt();
    _function_param["allow_mzero"] = sIniSetting->value("function_param/allow_mzero").toInt();
    _function_param["interface_theme"] = sIniSetting->value("function_param/interface_theme").toInt();
    _function_param["engine_type"] = sIniSetting->value("function_param/engine_type").toInt();
    _function_param["gear_type"] = sIniSetting->value("function_param/gear_type").toInt();
    _function_param["passwd"] = sIniSetting->value("function_param/passwd").toInt();
    _function_param["version"] = sIniSetting->value("function_param/version").toInt();
    _function_param["milage_total"] = sIniSetting->value("function_param/milage_total").toInt();
    _function_param["milage_part"] = sIniSetting->value("function_param/milage_part").toInt();
    _function_param["engine_time"] = sIniSetting->value("function_param/engine_time").toInt();

    _function_param["year"] = sIniSetting->value("function_param/year").toInt();
    _function_param["month"] = sIniSetting->value("function_param/month").toInt();
    _function_param["day"] = sIniSetting->value("function_param/day").toInt();
    _function_param["hour"] = sIniSetting->value("function_param/hour").toInt();
    _function_param["minute"] = sIniSetting->value("function_param/minute").toInt();
    _function_param["second"] = sIniSetting->value("function_param/second").toInt();
    _function_param["oil_bias"] = sIniSetting->value("function_param/oil_bias").toInt();
    _function_param["rspeed_bias"] = sIniSetting->value("function_param/rspeed_bias").toInt();
    _function_param["milage_part"] = sIniSetting->value("function_param/milage_part").toInt();
    _function_param["wtemp_bias"] = sIniSetting->value("function_param/wtemp_bias").toInt();

    //***其他仪表功能参数
    _function_param["low_speed"] = sIniSetting->value("function_param_others/low_speed").toInt();

    //***仪表功能测试点参数
    s = sIniSetting->value("function_test_point/meter_speed").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _function_tpoint["meter_speed"][i] = s[i].toInt();

    s = sIniSetting->value("function_test_point/meter_rspeed").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _function_tpoint["meter_rspeed"][i] = s[i].toInt();

    s = sIniSetting->value("function_test_point/meter_oil").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _function_tpoint["meter_oil"][i] = s[i].toInt();

    s = sIniSetting->value("function_test_point/meter_wtemp").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _function_tpoint["meter_wtemp"][i] = s[i].toInt();

    s = sIniSetting->value("function_test_point/pressure_brake1").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _function_tpoint["pressure_brake1"][i] = s[i].toInt();

    s = sIniSetting->value("function_test_point/pressure_brake2").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _function_tpoint["pressure_brake2"][i] = s[i].toInt();

    s = sIniSetting->value("function_test_point/pressure_brake3").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _function_tpoint["pressure_brake3"][i] = s[i].toInt();

    s = sIniSetting->value("function_test_point/pressure_break4").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
         _function_tpoint["pressure_break4"][i] = s[i].toInt();

    s = sIniSetting->value("function_test_point/pressure_oil").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
         _function_tpoint["pressure_oil"][i] = s[i].toInt();

    s = sIniSetting->value("function_test_point/intake_wtemp").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _function_tpoint["intake_wtemp"][i] = s[i].toInt();

    s = sIniSetting->value("function_test_point/voltage").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _function_tpoint["voltage"][i] = s[i].toInt();

    s = sIniSetting->value("function_test_point/brake_voltage").toString().split(" ",QString::SkipEmptyParts);
    for(i=0;i<s.size();i++)
        _function_tpoint["brake_voltage"][i] = s[i].toInt();

    range_current_min = sIniSetting->value("analog_channel/range_current_min").toInt();
    range_current_max = sIniSetting->value("analog_channel/range_current_max").toInt();
    range_resistor_min = sIniSetting->value("analog_channel/range_resistor_min").toInt();
    range_resistor_max = sIniSetting->value("analog_channel/range_resistor_max").toInt();
    range_voltage_min = sIniSetting->value("analog_channel/range_voltage_min").toInt();
    range_voltage_max = sIniSetting->value("analog_channel/range_voltage_max").toInt();
    range_power_min = sIniSetting->value("analog_channel/range_power_min").toInt();
    range_power_max = sIniSetting->value("analog_channel/range_power_max").toInt();

    if(_channel_type.size()!=0)
        return 1;
    else
        return 0;
}
