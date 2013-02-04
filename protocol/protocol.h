#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QMap>
#include <QVector>

//***时间
const   int     _MS_10_                         = 10;
const   int     _MS_50_                         = 50;
const   int     _MS_100_                        = 100;
const   int     _MS_200_                        = 200;
const   int     _MS_300_                        = 300;
const   int     _MS_500_                        = 500;
const   int     _MS_1000_                       = 1000;
const   int     _MS_2000_                       = 2000;
const   int     _MS_3000_                       = 3000;
const   int     _MS_4000_                       = 4000;
const   int     _MS_IDLE_                       = 5000;
const   int     _COM_TIME_OUT_                  = 3;

//***仪表模式
const   quint8  _METER_JOB_MODE_BOOTLOADER_     = 0x01;
const   quint8  _METER_JOB_MODE_FULLTEST_       = 0x02;
const   quint8  _METER_JOB_MODE_ADJUSTTEST_     = 0x03;

//***数据帧
const   quint8  _DLE_BYTE_ = 0x10;
const   quint8  _STX_BYTE_ = 0x02;
const   quint8  _ETX_BYTE_ = 0x03;

//***命令帧号
const   quint8  _METER_FRAME_REQUEST_               = 0x01;
const   quint8  _METER_FRAME_MODE_SETTING_          = 0x11;
const   quint8  _METER_FRAME_HARDWARE_TEST_         = 0x30;
const   quint8  _METER_FRAME_HRESPONSE_TEST_        = 0x20;
const   quint8  _METER_FRAME_COMMAND_ADJUST_        = 0x40;
const   quint8  _METER_FRAME_CRESPONSE_ADJUST_      = 0x50;
const   quint8  _METER_FRAME_INIT_PARA_ADJUST_      = 0x41;
const   quint8  _METER_FRAME_INIT_PRESPONSE_ADJUST_ = 0x51;
const   quint8  _METER_FRAME_SAVE_PARA_SPEED_ADJUST_    =0x42;
const   quint8  _METER_FRAME_SAVE_PARA_RSPEED_ADJUST_   =0x43;
const   quint8  _METER_FRAME_SAVE_PARA_OIL_ADJUST_      =0x44;
const   quint8  _METER_FRAME_SAVE_PARA_WTEMP_ADJUST_    =0x45;
const   quint8  _METER_FRAME_SAVE_PARA_CURRENT_ADJUST_    =0x46;
const   quint8  _METER_FRAME_SAVE_PARA_RESISTOR_ADJUST_    =0x47;
const   quint8  _METER_FRAME_SAVE_PARA_VOLTAGE_ADJUST_    =0x48;
const   quint8  _METER_FRAME_SAVE_PARA_POWER_ADJUST_    =0x49;
const   quint8  _METER_FRAME_RSAVE_PARA_SPEED_ADJUST_    =0x52;
const   quint8  _METER_FRAME_RSAVE_PARA_RSPEED_ADJUST_   =0x53;
const   quint8  _METER_FRAME_RSAVE_PARA_OIL_ADJUST_      =0x54;
const   quint8  _METER_FRAME_RSAVE_PARA_WTEMP_ADJUST_    =0x55;
const	quint8	_METER_FRAME_RSAVE_PARA_CURRENT_ADJUST_   =0x56;
const   quint8  _METER_FRAME_RSAVE_PARA_RESISTOR_ADJUST_    =0x57;
const   quint8  _METER_FRAME_RSAVE_PARA_VOLTAGE_ADJUST_    =0x58;
const   quint8  _METER_FRAME_RSAVE_PARA_POWER_ADJUST_    =0x59;
const   quint8  _METER_FRAME_SAVE_ADJUST_                =0x4F;
const   quint8  _METER_FRAME_RSAVE_ADJUST_                =0x5F;
const   quint8  _METER_FRAME_METER_PARA_SAVE_COMMAND_    =0x15;
const   quint8  _METER_FRAME_METER_PARA_RSAVE_COMMAND_    =0x05;
const   quint8  _METER_FRAME_METER_PARA_ASK_COMMAND_     =0x16;
const   quint8  _METER_FRAME_METER_PARA_RASK_COMMAND_    =0x06;


const   quint8  _PLC_FRAME_COMMAND_                     = 0x80;
const   quint8  _PLC_FRAME_CRESPONSE_                   = 0x81;

const	quint8	_MAX_METER_CHANNEL_                     = 4;
const	quint8	_MAX_ANALOG_CHANNEL_                    = 14;
const   quint8  _MAX_AADJUST_POINT_                     = 4;
const   quint8  _MAX_MADJUST_POINT_                     = 8;

const   quint8  _MAX_ATEST_POINT_                   = 10;


const   quint8  ATYPE_CURRENT                       = 0;
const   quint8  ATYPE_RESISTOR                      = 1;
const   quint8  ATYPE_VOLTAGE                       = 2;
const   quint8  ATYPE_POWER                         = 3;

const   quint8  BITMAP_NAME_COUNT                   = 54;
const   double  SAMPLE_ERROR                        = 0.05;


/******************************************单板硬件总检*******************************************/
//***硬件工作执行帧: 24bytes 0x30
struct hardwareTestFrame_0x30
{
    quint8  cmd;
    quint8  command[3];
    quint8  can_id[4];
    quint8  can_data[8];
    quint8  year[2];
    quint8  month;
    quint8  day;
    quint8  hour;
    quint8  minute;
    quint8  second;
};

//***硬件工作执行反馈帧: 93bytes 0x20
struct hardwareTestRespondFrame_0x20
{
    //***模拟量
    quint16 ao[10];
    quint16 pw[4];

    //***频率量
    quint32 fq[2];

    //***IO量
    quint32 ioi;
    quint8  ioo;

    //*** LED
    quint32 led;
    quint8  led_light;
    quint8  lcd_light;

    //***Key
    quint8  key;

    //***CAN
    quint32 can0_count;
    quint32 can0_id;
    quint8  can0_data[8];
    quint32 can1_count;
    quint32 can1_id;
    quint8  can1_data[8];

    //***步进电机
    quint16 motor0;
    quint16 motor1;
    quint16 motor2;
    quint16 motor3;

    //***测试结果
    quint8  result;
    quint8  control;
    //***CPU温度
    quint16 cpu_temp;
};

//***调校命令帧: 28bytes 0x40
struct adjustCommandFrame_0x40
{
    quint8  cmd;
    quint8  id;
    quint8  command;
    quint8  motor;
    quint8  step[2];
    quint8  channel;
};

//***调校命令返回帧: 28bytes 0x50
struct adjustCResponseFrame_0x50
{
    quint8  id;
    qint16 sample;
};

//***调校初始参数操作帧: 28bytes 0x41
struct adjustInitParaFrame_0x41
{
    quint8  cmd;
    quint8  id;
    quint8  type;
    quint8  date[4];
    quint8  version;
    quint8  version1;
    quint8  num_meter[_MAX_METER_CHANNEL_];
    quint8  num_analog[_MAX_ANALOG_CHANNEL_];
    quint8  rcof;
};

//***调校初始参数应答帧: 27bytes 0x51
struct adjustInitPResponseFrame_0x51
{
    quint8  cmd;
    quint8  id;
    quint32 date;
    quint8  version;
    quint8  version1;
    quint8  num_meter[_MAX_METER_CHANNEL_];
    quint8  num_analog[_MAX_ANALOG_CHANNEL_];
    quint8  rcof;
};

//***仪表参数存储帧: 28bytes 0x42～0x45
struct adjustSaveParaFrame_0x42_0x45
{
    quint8  cmd;
    quint8  id;
    quint8  command;
    quint8  number;
    qint8   sample0[2];
    quint8  value0[2];
    qint8   sample1[2];
    quint8  value1[2];
    qint8   sample2[2];
    quint8  value2[2];
    qint8   sample3[2];
    quint8  value3[2];
    qint8   sample4[2];
    quint8  value4[2];
    qint8   sample5[2];
    quint8  value5[2];
    qint8   sample6[2];
    quint8  value6[2];
    qint8   sample7[2];
    quint8  value7[2];
};

//***仪表参数存储返回帧: 28bytes 0x52～0x55
struct adjustSPResponseFrame_0x52_0x55
{
    quint8  id;
    quint8  start;
    quint8  number;
    quint16 value[_MAX_MADJUST_POINT_];
    qint16  sample[_MAX_MADJUST_POINT_];
};

//***仪表参数存储帧: 28bytes 0x46～0x49
struct adjustSaveParaFrame_0x46_0x49
{
    quint8  cmd;
    quint8  id;
    quint8  command1;
    quint8  command2;
    quint8  data[4*_MAX_MADJUST_POINT_];
};

//***仪表参数存储帧: 28bytes 0x46～0x49
struct adjustSaveParaFrame_0x56_0x59 
{
    quint8  cmd;
    quint8  id;
    quint8  start;
    quint8  num;
	quint8	channel;
    quint16 value[_MAX_MADJUST_POINT_];
    quint16 sample[_MAX_MADJUST_POINT_];
};

//***调校参数存储帧
struct adjustSaveFrame_0x4F
{
    quint8  cmd;
    quint8  id;
};

struct adjustRSaveFrame_0x5F
{
    quint8  cmd;
    quint8  id;
    quint8  fid;
    quint8  time;
};

typedef QMap<int,quint16>    vu16;
typedef QMap<int,qint16>     v16;


//***PLC命令帧:
struct PLCCommandFrame_0x80
{
    quint8  cmd;
    quint8  id;
    //***电源
    quint8  _12v;
    quint8  _xxv;
    //***IO输出
    quint8  ioo[10];
    quint8  ifeedback;
    //***IO输入
    quint8  ioi[2];
    //***电流输出
    quint8  curout[8];
    //***电阻输出
    quint8  resout[4];
    //***电压输出
    quint8  vout[8];
    //***
    quint8  freqout[8];
    quint8  freqvout[4];

    //***Can
    quint8  can1_id[4];
    quint8  can1_data[8];
    quint8  can1_cmd;
    quint8  can1_rate[2];
    quint8  can2_id[4];
    quint8  can2_data[8];
    quint8  can2_cmd;
    quint8  can2_rate[2];
};

//***PLC返回帧:
struct PLCCResponseFrame_0x81
{
    quint8  cmd;
    quint8  id;

    //***IO输入
    quint8  ioi[3];
    //***电压输入
    quint8  vin[8];
    //***频率输入
    quint16  freqin[4];
    //***Can
    quint32 can1_id;
    quint8  can1_data[8];
    quint16 can1_num;
    quint32 can2_id;
    quint8  can2_data[8];
    quint16 can2_num;
};

//***仪表参数存储帧
struct MeterSettingFrame_0x15
{
    quint8  cmd;
    quint8  command[4];

    quint8  speed_pulse[2];
    quint8  w_cwater;
    quint8  w_lpressure[2];
    quint8  w_opressure;
    quint8  w_pressurel[2];
    quint8  backlight;
    quint8  ledlight;
    quint8  fan_lspeed_temp;
    quint8  fan_hspeed_temp;
    quint8  fan_lspeed_wtemp;
    quint8  fan_hspeed_wtemp;
    quint8  fan_lspeed_diff;
    quint8  fan_hspeed_diff;
    quint8  param_setting;
    quint8  language;
    quint8  loil_type;
    quint8  brake_type;
    quint8  brake_valid;
    quint8  allow_mzero;
    quint8  interface_theme[2];
    quint8  engine_type;
    quint8  gear_type;
    quint8  passwd[5];
    quint8  version;

    quint8  milage_all[4];
    quint8  milage_part[4];
    quint8  engine_time[4];

    quint8  year[2];
    quint8  month;
    quint8  day;
    quint8  hour;
    quint8  minute;
    quint8  second;

    qint8   bstep_oil[2];
    qint8   bstep_rspeed[2];
    qint8   bstep_wtemp[2];
    qint8   bstep_speed[2];
};

struct MeterSettingReturnFrame_0x05
{
    quint8  cmd;
};

struct MeterAskFrame_0x16
{
    quint8  cmd;
};

//***仪表参数存储返回帧
struct MeterAskReturnFrame_0x06
{
    quint8  cmd;

    quint16 speed_pulse;
    quint8  w_cwater;
    quint16 w_lpressure;
    quint8  w_opressure;
    quint16 w_pressurel;
    quint8  backlight;
    quint8  ledlight;
    quint8  fan_lspeed_temp;
    quint8  fan_hspeed_temp;
    quint8  fan_lspeed_wtemp;
    quint8  fan_hspeed_wtemp;
    quint8  fan_lspeed_diff;
    quint8  fan_hspeed_diff;
    quint8  param_setting;
    quint8  language;
    quint8  loil_type;
    quint8  brake_type;
    quint8  brake_valid;
    quint8  allow_mzero;
    quint16 interface_theme;
    quint8  engine_type;
    quint8  gear_type;
    quint8  passwd[5];
    quint8  version;

    quint32 milage_all;
    quint32 milage_part;
    quint32 engine_time;

    qint16  bstep_oil;
    qint16  bstep_rspeed;
    qint16  bstep_wtemp;
    qint16  bstep_speed;
};


#define     NONE_ID                 -1
#define     SETTING_ID              0
#define     INITTEST_ID             1
#define     BOOTLOADER_ID           2
#define     FULLTEST_ID             3
#define     ADJUSTTEST_ID           4
#define     FUNCTIONTEST_ID         5
#define     SUMMARY_ID              6

#define     INIT_TEST_SHORT_ID          0
#define     INIT_TEST_VOLTAGE_ID        1
#define     INIT_TEST_RESISTOR_ID       2
#define     INIT_TEST_BOOTLOADER_ID     3

#define     FULL_TEST_LED_ID            0
#define     FULL_TEST_LCD_BACKLIGHT_ID  1
#define     FULL_TEST_LCD_ID            2
#define     FULL_TEST_KEY_ID            3
#define     FULL_TEST_MEMORY_ID         4
#define     FULL_TEST_SELF_POWER_ID     5
#define     FULL_TEST_METER_VOLTAGE_ID  6
#define     FULL_TEST_MOTOR_ID          7
#define     FULL_TEST_CAN_ID            8
#define     FULL_TEST_IO_IN_ID          9
#define     FULL_TEST_IO_OUT_ID         10
#define     FULL_TEST_ANALOG_ID         11
#define     FULL_TEST_FREQ_ID           12

#define     ADJUST_TEST_SPEED_ID        0
#define     ADJUST_TEST_RSPEED_ID       1
#define     ADJUST_TEST_OIL_ID          2
#define     ADJUST_TEST_WATERTEMP_ID    3
#define     ADJUST_TEST_ANALOG_ID       4
/*
#define     FUNCTION_TEST_LED_ID            0
#define     FUNCTION_TEST_SPEED_ID          1
#define     FUNCTION_TEST_RSPEED_ID         2
#define     FUNCTION_TEST_OIL_ID            3
#define     FUNCTION_TEST_WTEMP_ID          4
#define     FUNCTION_TEST_PRESSURE_ID       5
#define     FUNCTION_TEST_OILPRESSURE_ID    6
#define     FUNCTION_TEST_INTEMP_ID         7
#define     FUNCTION_TEST_VOLTAGE_ID        8
*/
#define     FUNCTION_TEST_SPEED_ID          0
#define     FUNCTION_TEST_RSPEED_ID         1
#define     FUNCTION_TEST_OIL_ID            2
#define     FUNCTION_TEST_WTEMP_ID          3
#define     FUNCTION_TEST_PRESSURE_ID       4
#define     FUNCTION_TEST_OILPRESSURE_ID    5
#define     FUNCTION_TEST_INTEMP_ID         6
#define     FUNCTION_TEST_VOLTAGE_ID        7
#define     FUNCTION_TEST_LED_ID            10
struct testRecord
{
    quint32 _id;
    QString _meter;
    QString _user;
    quint32 _date;
    quint32 _inittest;
    quint32 _fulltest;
    quint32 _adjusttest;
    quint32 _functiontest;
    QString _comment;
};

struct testPointRecord
{

};

bool getTestBit(quint32 test,int id);
void setTestBit(quint32& test,int id);
void clearTestBit(quint32& test,int id);
double getTestSucessPercentage(int num,int test);

hardwareTestRespondFrame_0x20 frame20FromData(QByteArray a);
adjustCResponseFrame_0x50 frame50FromData(QByteArray a);
adjustInitPResponseFrame_0x51 frame51FromData(QByteArray a);
adjustSPResponseFrame_0x52_0x55 frame52_55FromData(QByteArray a);
adjustSaveParaFrame_0x56_0x59 frame56_59FromData(QByteArray a);
PLCCResponseFrame_0x81 frame81FromData(QByteArray a);
adjustRSaveFrame_0x5F frame5FFromData(QByteArray a);
MeterAskReturnFrame_0x06 frame06FromData(QByteArray a);

QByteArray data30FromFrame(hardwareTestFrame_0x30 b);
QByteArray data80FromFrame(PLCCommandFrame_0x80 b);
QByteArray data15FromFrame(MeterSettingFrame_0x15 b);
QByteArray data16FromFrame(MeterAskFrame_0x16 b);

void setBit(quint8& data,quint8 bit);
void clearBit(quint8& data,quint8 bit);
bool getBit(quint8 data,quint8 bit);

quint8 charToHex(quint8 s);
quint8 numToChar(quint8 s);
QByteArray stringToHex(QString);

QByteArray hexToString(quint8*);

quint8 hexToChar (quint8 s);
QByteArray arrangeByte(int val,int num);
QByteArray arrangeByteO(int val,int num);
quint8 calResistor(int a);
int getTheorySample(int min,int max,int val,int range);
int getTheorySampleOld(int min,int max,int val,int range);
QByteArray splitTime(int val);
int getPressure(int val);
int getOilResistor(int val);
QByteArray getDateBCD();

template <typename T>
QString mapToString(QMap<int,T> a)
{
    QString s="";

    for(int i=0;i<a.size();i++)
    {
        if(i!=a.size()-1)
            s+=QString::number(a[i]) + " ";
        else
            s+=QString::number(a[i]);
    }

    return s;
}

#endif
