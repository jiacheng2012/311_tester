#include <QDebug>
#include "cdataframe.h"
#include "protocol.h"

CDataFrame::CDataFrame()
{
}

/************************************************************************************
//INT8U CRC8_Get(U8* buf, U8 len)
//查表法计算CRC8
//INT8U* buf:		缓冲
//INT8U len:		缓冲长度
//返回:			对缓冲计算得到的CRC8值
//注意:			初值为定制版的0xAA
************************************************************************************/
quint8 CDataFrame::CRC8_Get(quint8* buf, quint8 len)
{
    const char CRC8_TAB[] =
    {
        0,94,188,226,97,63,221,131,194,156,126,32,163,253,31,65,
        157,195,33,127,252,162,64,30, 95,1,227,189,62,96,130,220,
        35,125,159,193,66,28,254,160,225,191,93,3,128,222,60,98,
        190,224,2,92,223,129,99,61,124,34,192,158,29,67,161,255,
        70,24,250,164,39,121,155,197,132,218,56,102,229,187,89,7,
        219,133,103,57,186,228,6,88,25,71,165,251,120,38,196,154,
        101,59,217,135,4,90,184,230,167,249,27,69,198,152,122,36,
        248,166,68,26,153,199,37,123,58,100,134,216,91,5,231,185,
        140,210,48,110,237,179,81,15,78,16,242,172,47,113,147,205,
        17,79,173,243,112,46,204,146,211,141,111,49,178,236,14,80,
        175,241,19,77,206,144,114,44,109,51,209,143,12,82,176,238,
        50,108,142,208,83,13,239,177,240,174,76,18,145,207,45,115,
        202,148,118,40,171,245,23,73,8,86,180,234,105,55,213,139,
        87,9,235,181,54,104,138,212,149,203, 41,119,244,170,72,22,
        233,183,85,11,136,214,52,106,43,117,151,201,74,20,246,168,
        116,42,200,150,21,75,169,247,182,232,10,84,215,137,107,53
    };

    quint8 CRC8 = 0xAA, *ptr = buf;
    quint8 cnt = len;
    while(cnt--)
    {
        CRC8 = CRC8_TAB[(*ptr) ^ CRC8];
        ptr++;
    }
    return CRC8;
}


bool CDataFrame::inputData(QByteArray a)
{
    int num = a.size();
    char *b=a.data();

    if(num<6)
        return false;
    else if( a[0]!=_DLE_BYTE_ || a[1]!=_STX_BYTE_ || a[num-2]!=_DLE_BYTE_ || a[num-1]!=_ETX_BYTE_ )
        return false;
    else
    {
        //***获取命令
        _command = a[2];
        //***获取CRC
        _crc = a[num-3];
        QByteArray t1 = a.mid(2,num-4);
        if(CRC8_Get((uchar *)(t1.data()),t1.size())!=0)
            return false;
        //***处理转义字符
        QByteArray t2 = a.mid(3,num-6);
/*        for(int i=0;i<t2.size()-1;i++)
        {
            if(t2[i] == _DLE_BYTE_ && t2[i+1] == _DLE_BYTE_)
                t2.remove(i,_DLE_BYTE_);
        }
*/
        _data = t2;
        return true;
    }
}

bool CDataFrame::setupDataFrame(QByteArray &a)
{
    int num = a.size();
    if(num<1)
    {
        return false;
    }
    else
    {
        //***计算CRC检验码
        _crc = CRC8_Get((uchar*)(a.data()),a.size());

        _command = a[0];
        //***处理转义字符
        QByteArray t1= a.mid(1,num-1);
        for(int i=0;i<t1.size();i++)
        {
            if(t1[i] == _DLE_BYTE_)
            {
                t1.insert(i,_DLE_BYTE_);
                i++;
            }
        }
        _data = t1;
        return true;
    }
}

QByteArray CDataFrame::formatOutputDataStream()
{
    QByteArray  d;

    //***填充数据帧
    int size = _data.size();
    d.insert(0,_DLE_BYTE_);
    d.insert(1,_STX_BYTE_);
    d.insert(2,_command);
    d.insert(3,_data);
    d.insert(3+size,_crc);
    d.insert(4+size,_DLE_BYTE_);
    d.insert(5+size,_ETX_BYTE_);

    return d;
}

QByteArray CDataFrame::getData()
{
    return _data;
}

quint8 CDataFrame::getSize()
{
    return _data.size();
}

quint8 CDataFrame::getType()
{
    return _command;
}
