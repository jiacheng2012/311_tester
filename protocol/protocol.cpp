#include <QStringList>
#include "protocol.h"
#include <QDebug>
#include <QDateTime>

bool getTestBit(quint32 test,int id)
{
    return test&(1<<id);
}

void setTestBit(quint32& test,int id)
{
    test = test | (1<<id);
}

void clearTestBit(quint32& test,int id)
{
    test = test & (~(1<<id));
}

double getTestSucessPercentage(int num,int test)
{
    int n=0;
    for(int i=0;i<num;i++)
    {
        if(test&(1<<i))
            n++;
    }

    return (double)n/(double)num;
}

hardwareTestRespondFrame_0x20 frame20FromData(QByteArray a)
{
    quint8  b[92];
    int i;

    hardwareTestRespondFrame_0x20 t;

    for(i=0;i<92;i++)
        b[i] = (quint8)a[i];

    t.ao[0] = b[0] + b[1]*256;
    t.ao[1] = b[2] + b[3]*256;
    t.ao[2] = b[4] + b[5]*256;
    t.ao[3] = b[6] + b[7]*256;
    t.ao[4] = b[8] + b[9]*256;
    t.ao[5] = b[10] + b[11]*256;
    t.ao[6] = b[12] + b[13]*256;
    t.ao[7] = b[14] + b[15]*256;
    t.ao[8] = b[16] + b[17]*256;
    t.ao[9] = b[18] + b[19]*256;

    t.pw[0] = b[20] + b[21]*256;
    t.pw[1] = b[22] + b[23]*256;
    t.pw[2] = b[24] + b[25]*256;
    t.pw[3] = b[26] + b[27]*256;

    t.fq[0] = b[28] + b[29]*256 + b[30]*256*256 + b[31]*256*256*256;
    t.fq[1] = b[32] + b[33]*256 + b[34]*256*256 + b[35]*256*256*256;
    t.ioi = b[36] + b[37]*256 + b[38]*256*256 + b[39]*256*256*256;
    t.ioo = b[40];

    t.led = b[41] + b[42]*256 + b[43]*256*256 + b[44]*256*256*256;
    t.led_light = b[45];
    t.lcd_light = b[46];
    t.key = b[47];

    t.can0_count = b[48] + b[49]*256 + b[50]*256*256 + b[51]*256*256*256;
    t.can0_id = b[52] + b[53]*256 + b[54]*256*256 + b[55]*256*256*256;
    for(i=0;i<8;i++)
        t.can0_data[i] = b[56+i];

    t.can1_count = b[64] + b[65]*256 + b[66]*256*256 + b[67]*256*256*256;
    t.can1_id = b[68] + b[69]*256 + b[70]*256*256 + b[71]*256*256*256;
    for(i=0;i<8;i++)
        t.can1_data[i] = b[72+i];

    t.motor0 = b[80] + b[81]*256;
    t.motor1 = b[82] + b[83]*256;
    t.motor2 = b[84] + b[85]*256;
    t.motor3 = b[86] + b[87]*256;

    t.result = b[88];
    t.control = b[89];
    t.cpu_temp = b[90] + b[91]*256;
    return t;
}

adjustCResponseFrame_0x50 frame50FromData(QByteArray a)
{
    int i;
    adjustCResponseFrame_0x50 t;

    quint8 b[3];
    for(i=0;i<3;i++)
        b[i] = (quint8)a[i];

    t.id = b[0];
    t.sample = b[1] + b[2]*256;
    return t;
}

adjustInitPResponseFrame_0x51 frame51FromData(QByteArray a)
{
    int i;
    adjustInitPResponseFrame_0x51 t;

    quint8 b[26];
    for(i=0;i<26;i++)
        b[i] = (quint8)a[i];

    t.id = b[0];
    t.date = b[1] + b[2]*256 + b[3]*256*256 + b[4]*256*256*256;
    t.version = b[5];
    t.version1 = b[6];

    t.num_meter[0] = b[7];
    t.num_meter[1] = b[8];
    t.num_meter[2] = b[9];
    t.num_meter[3] = b[10];
    t.num_analog[0] = b[11];
    t.num_analog[1] = b[12];
    t.num_analog[2] = b[13];
    t.num_analog[3] = b[14];
    t.num_analog[4] = b[15];
    t.num_analog[5] = b[16];
    t.num_analog[6] = b[17];
    t.num_analog[7] = b[18];
    t.num_analog[8] = b[19];
    t.num_analog[9] = b[20];
    t.num_analog[10] = b[21];
    t.num_analog[11] = b[22];
    t.num_analog[12] = b[23];
    t.num_analog[13] = b[24];
    t.rcof = b[25];

    return t;
}

adjustSPResponseFrame_0x52_0x55 frame52_55FromData(QByteArray a)
{
    int i;
    adjustSPResponseFrame_0x52_0x55 t;

    t.id = a[0];
    t.start = 0;//a[1];
    t.number = a[2];

    int start = 0;
    int num = t.number;
    for(i=start;i<(start+num);i++)
    {
        t.sample[i] = (qint8)a[3+4*i] + (qint8)(a[4+4*i])*256;
        t.value[i] = (quint8)a[5+4*i] + (quint8)(a[6+4*i])*256;
    }
    return t;
}

adjustSaveParaFrame_0x56_0x59 frame56_59FromData(QByteArray a)
{
    int i;
    adjustSaveParaFrame_0x56_0x59 t;

    t.id = a[0];
    t.start = a[1];
    t.num = (a[2]>>4)&0x0F;
    t.channel = a[2]&0x0F;

    quint8 b[52];
    for(i=0;i<4*t.num;i++)
        b[i] = (quint8)a[3+i];

    int start = t.start;
    int num = t.num;
    for(i=start;i<(start+num);i++)
    {
        t.value[i] = b[4*i] + b[1+4*i]*256;
        t.sample[i] = b[2+4*i] + b[3+4*i]*256;
    }
    return t;
}

adjustRSaveFrame_0x5F frame5FFromData(QByteArray a)
{
    int i;
    adjustRSaveFrame_0x5F t;

    quint8 b[3];
    for(i=0;i<a.size();i++)
        b[i] = (quint8)a[i];

    t.id = b[0];
    t.fid = b[1];
    t.time = b[2];

    return t;
}

PLCCResponseFrame_0x81 frame81FromData(QByteArray a)
{
    int i;
    PLCCResponseFrame_0x81 t;
    quint8 b[52];

    for(i=0;i<a.size(); i++)
        b[i] =  (quint8)a[i];

    t.id = b[0];
    t.ioi[0] = b[1];
    t.ioi[1] = b[2];
    t.ioi[2] = b[3];

    for(i=0;i<8;i++)
        t.vin[i] = b[4+i];

    for(i=0;i<4;i++)
        t.freqin[i]= b[i*2+12] + b[i*2+13]*256;

    t.can1_id = b[20] + b[21]*256 + b[22]*256*256 + b[23]*256*256*256;
    for(i=0;i<8;i++)
        t.can1_data[i] = b[24+i];
    t.can1_num = b[32] + b[33]*256;

    t.can2_id = b[34] + b[35]*256 + b[36]*256*256 + b[37]*256*256*256;
    for(i=0;i<8;i++)
        t.can2_data[i] = b[38+i];
    t.can2_num = b[46] + b[47]*256;

    return t;
}


MeterAskReturnFrame_0x06 frame06FromData(QByteArray a)
{
    int i;
    MeterAskReturnFrame_0x06 t;

    quint8 b[53];
    for(i=0;i<53;i++)
        b[i] = (quint8)a[i];

    t.speed_pulse = b[0] + b[1]*256;
    t.w_cwater = b[2];
    t.w_lpressure = b[3] + b[4]*256;
    t.w_opressure = b[5];
    t.w_pressurel = b[6] + b[7]*256;
    t.backlight = b[8];
    t.ledlight = b[9];
    t.fan_lspeed_temp = b[10];
    t.fan_hspeed_temp = b[11];
    t.fan_lspeed_wtemp = b[12];
    t.fan_hspeed_wtemp = b[13];
    t.fan_lspeed_diff = b[14];
    t.fan_hspeed_diff = b[15];
    t.param_setting = b[16];
    t.language = b[17];
    t.loil_type = b[18];
    t.brake_type = b[19];
    t.brake_valid = b[20];
    t.allow_mzero = b[21];
    t.interface_theme = b[22] + b[23]*256;
    t.engine_type = b[24];
    t.gear_type = b[25];
    for(i=0;i<5;i++)
        t.passwd[i] = b[26+i];

    t.version = b[31];
    t.milage_all = b[32] + b[33]*256 + b[34]*256*256 + b[35]*256*256*256;
    t.milage_part = b[36] + b[37]*256 + b[38]*256*256 + b[39]*256*256*256;
    t.engine_time = b[40] + b[41]*256 + b[42]*256*256 + b[43]*256*256*256;

    t.bstep_oil = b[44] + b[45]*256;
    t.bstep_rspeed = b[46] + b[47]*256;
    t.bstep_wtemp = b[48] + b[49]*256;
    t.bstep_speed = b[50] + b[51]*256;

    return t;
}

QByteArray data30FromFrame(hardwareTestFrame_0x30 b)
{
    QByteArray a=QByteArray::fromRawData((const char*)&b,sizeof(b));
    return a;
}

QByteArray data80FromFrame(PLCCommandFrame_0x80 b)
{
    QByteArray a=QByteArray::fromRawData((const char*)&b,sizeof(b));
    return a;
}


QByteArray data15FromFrame(MeterSettingFrame_0x15 b)
{
    QByteArray a=QByteArray::fromRawData((const char*)&b,sizeof(b));
    return a;
}

QByteArray data16FromFrame(MeterAskFrame_0x16 b)
{
    QByteArray a;
    a[0] = b.cmd;
    return a;
}

void setBit(quint8& data,quint8 bit)
{
    if(bit<0 || bit>7)
        return;

    quint8 mask=1<<bit;

    data = data|mask;
}

void clearBit(quint8& data,quint8 bit)
{
    if(bit<0 || bit>7)
        return;

    quint8 mask=1<<bit;

    data = data &(~mask);
}

bool getBit(quint8 data,quint8 bit)
{
    if(bit<0 || bit>7)
        return false;

    quint8 mask=1<<bit;

    return data & mask;

}

QByteArray stringToHex(QString s)
{
    int i;
    QStringList s1 = s.split(" ",QString::SkipEmptyParts);
    QString s2="";
    for(i=0;i<s1.size();i++)
        s2+=s1.at(i);

    QByteArray a = s2.toAscii();

    for(i=0;i<a.size();i++)
        a[i] = charToHex(a.at(i));

    return a;
}

QByteArray hexToString(quint8 *s)
{
    QByteArray a;
    int i;
    for(i=0;i<8;i++)
    {
        int b = s[i]/16;
        int c = s[i] - b*16;

        a[3*i] = hexToChar(b);
        a[3*i+1] = hexToChar(c);
        if(i!=7)
            a[3*i+2] = 32;
    }

    return a;
}

quint8 charToHex(quint8 s)
{
    quint8 value;
    if( (s >= '0') && (s <= '9') )
    {
        value = s - 0x30;
    }else if( (s >= 'A') && (s <= 'F') )
    {
        value = s - 0x37;
    }else if( (s >= 'a') && (s <= 'f') )
    {
        value = s - 0x57;
    }else
    {
        value = 0x0;
    }

    return value;
}

quint8 numToChar(quint8 s)
{
    if( (s >= 48) && (s <= 57) )
    {
        s-=48;
    }
    else
    {
        s = 48;
    }

    return s;
}

quint8 hexToChar (quint8 s)
{
    quint8 value;
    if( (s >= 0) && (s <= 9) )
    {
        value = s + 0x30;
    }else if( (s >= 10) && (s <= 15) )
    {
        value = s + 0x37;
    }else
    {
        value = 0x0;
    }

    return value;
}
QByteArray arrangeByte(int val,int num)
{
    QByteArray a;

    if(val>=0)
    {
        a = arrangeByteO(val,num);
    }
    else
    {
        int tmp = val*-1;
        a = arrangeByteO(tmp,num);

        if(num==2)
        {
            int t1 = ~a[0] + 1;
            int t2 = ~a[1];
            if(t1>=255)
            {
                t1 = 0;
                t2+= 1;
            }
            a[0] = t1;
            a[1] = t2;
        }
    }

    return a;
}

QByteArray arrangeByteO(int val,int num)
{
    QByteArray a;

    if(num==1)
    {
        a[0] = val;
    }
    else if(num==2)
    {
        int d = (int)(val/256);
        a[0] = val - d*256;
        a[1] = d;
    }
    else if(num==3)
    {
        int d = (int)(val/65536);
        int e = (int)((val - d*65536)/256);

        a[0] = val - d*65536 - e*256;
        a[1] = e;
        a[2] = d;
    }
    else if(num==4)
    {
        int d = (int)(val/16777216);
        int e = (int)((val - d*16777216)/65536);
        int f = (int)((val - d*16777216 - e*65536 )/256);

        a[0] = val - d*16777216 - e*65536 - f*256;
        a[1] = f;
        a[2] = e;
        a[3] = d;
    }
    else if(num==5)
    {
        int d = (int)(val/4294967296);
        int e = (int)((val - d*4294967296)/16777216);
        int f = (int)((val - d*4294967296 - e*16777216 )/65536);
        int g = (int)((val - d*4294967296 - e*16777216 - f*65536)/256);

        a[0] = val - d*4294967296 - e*16777216 - f*65536 - g*256;
        a[1] = g;
        a[2] = f;
        a[3] = e;
        a[4] = d;
    }
    return a;
}

quint8 calResistor(int a)
{
    quint8 b=0x0;

    if(a==10)
        b=0b11110;
    else if(a==50)
        b=0b11101;
    else if(a==60)
        b=0b11100;
    else if(a==100)
        b=0b11011;
    else if(a==110)
        b=0b11010;
    else if(a==150)
        b=0b10111;
    else if(a==160)
        b=0b10110;
    else if(a==180)
        b=0b01111;
    else if(a==190)
        b=0b01110;
    else if(a==200)
        b=0b10101;
    else if(a==210)
        b=0b10100;
    else if(a==230)
        b=0b01101;
    else if(a==240)
        b=0b01100;
    else if(a==260)
        b=0b10010;
    else if(a==280)
        b=0b01011;
    else if(a==290)
        b=0b01010;
    else if(a==300)
        b=0b10001;
    else if(a==310)
        b=0b10000;
    else if(a==330)
        b=0b00111;
    else if(a==340)
        b=0b00110;
    else if(a==380)
        b=0b00101;
    else if(a==390)
        b=0b00100;
    else if(a==430)
        b=0b00011;
    else if(a==440)
        b=0b00010;
    else if(a==490)
        b=0b00000;
    else
        b=0xff;

    return b;
}

int getTheorySample(int min,int max,int val,int range)
{
    double step = (double)range/((double)max-(double)min);

    return   (int)((val-min)*step);
}

QByteArray splitTime(int val)
{
    QByteArray a;

    a[0] = (int)(val/3600);
    a[1] = (int)((val - a[0]*3600)/60);
    a[2] = val - a[0]*3600 - a[1]*60;

    return a;

}

int getPressure(int val)
{
    double step = 16.0 / 1000000.0;
    double aa = (double)val;

    return 4+(int)aa*step;
}

int getOilResistor(int val)
{
    double step = 170.0 / 100.0;
    double aa = (double)val;

    return 10+(int)aa*step;
}

QByteArray getDateBCD()
{
    QByteArray dd;
    QDateTime a = QDateTime::currentDateTime();
    int f1,f2,f3,f4,f5,f6,f7,f8;

    f1 = (a.date().year())/1000;
    f2 = (a.date().year() - f1*1000)/100;
    f3 = (a.date().year() - f1*1000 - f2*100)/10;
    f4 =  a.date().year() - f1*1000 - f2*100 - f3;

    f5 = (a.date().month())/10;
    f6 =  a.date().month() - f5*10;

    f7 = (a.date().day())/10;
    f8 = a.date().day() - f7*10;

    dd[0] = f1 << 4 | f2;
    dd[1] = f3 << 4 | f3;

    dd[2] = f5 << 4 | f6;
    dd[3] = f6 << 4 | f7;

    return dd;
}
