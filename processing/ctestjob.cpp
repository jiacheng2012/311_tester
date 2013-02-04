#include "ctestjob.h"
#include "capp.h"

CTestJob::CTestJob(QString id,QObject *parent):
    QObject(parent)
{
    _jtype = NONE_ID;
    _ttype = NONE_ID;
    _status232 = 0;
    _status485 = 0;
    _232t = 0;
    _485t = 0;
    _aconfig = 0;
    _mconfig = 0;

    timerEvent(0);
}

void CTestJob::timerEvent(QTimerEvent* event)
{
    int tag = QTime::currentTime().secsTo(QTime(2012,1,1));
    if(_status232)
    {
        if( abs( tag - _232tag) > _COM_TIME_OUT_ )
        {
            _232t->_timer->stop();
            _232t->_port->Close();
            _232t->terminate();
            _232t->wait();
            _status232 = 0;
            killTimer(tid);
            emit stop232();
            emit newMessage(tr("失去仪表串口通讯！请重连接仪表！"),1);
        }
    }

    if(_status485)
    {
        if( abs( tag - _485tag) > _COM_TIME_OUT_ )
        {
            _485t->_timer->stop();
            _485t->_port->Close();
            _485t->terminate();
            _485t->wait();
            _status485 = 0;
            killTimer(tid);
            emit stop485();
            emit newMessage(tr("失去信号板串口通讯！请重连接信号板！"),1);
        }
    }
}

CTestJob::~CTestJob()
{
    _aconfig = 0;
    _mconfig = 0;

    delete232();
    delete485();
}

void CTestJob::clearJob()
{
    if(_jtype == FULLTEST_ID)
    {
        hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
        setupSendFrame232(data30FromFrame(b),_MS_200_);
    }
    else if(_jtype == ADJUSTTEST_ID)
    {
        adjustInitParaFrame_0x41 b={_METER_FRAME_INIT_PARA_ADJUST_,0x00};
        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        setupSendFrame232(tmp,_MS_300_);
    }
    else
    {
        MeterAskFrame_0x16 b={_METER_FRAME_METER_PARA_ASK_COMMAND_};
        setupSendFrame232(data16FromFrame(b),_MS_1000_);
    }
    //***clear 485
    PLCCommandFrame_0x80 c={0};
    initFrame80(&c);
    c._xxv = 0;
    QByteArray tmp=QByteArray::fromRawData((const char*)&c,sizeof(c));
    setupSendFrame485(tmp,_MS_300_);
    QTimer::singleShot(_MS_500_, this, SLOT(delayClearJob()));
}

void CTestJob::delayClearJob()
{
    //
    _jtype = NONE_ID;
    _ttype = NONE_ID;
    _status232 = 0;
    _status485 = 0;

    delete232();
    delete485();
}

void CTestJob::delete232()
{
    //***停止232线程
    if(_232t!=0)
    {
        _232t->deleteLater();
        _232t->quit();
        _232t->exit();
        _232t = 0;

        emit stop232();
        emit newMessage(tr("停止仪表串口线程，连接断开！"),0);
    }
}

void CTestJob::delete485()
{
    //***停止485线程
    if(_485t!=0)
    {
        _485t->deleteLater();
        _485t->quit();
        _485t->exit();
        _485t = 0;

        emit stop485();
        emit newMessage(tr("停止信号箱串口线程，连接断开！"),0);
    }
}

void CTestJob::connect232(int t)
{
    if(!_status232)
    {
        _status232 = 0;
        _232t = 0;
        _pool232.clear();
        _list232.clear();

        _232t = new C232Thread();
        connect(_232t, SIGNAL(newMessage(QString,bool)),this->parent(), SIGNAL(newMessage(QString,bool)));
        connect(_232t, SIGNAL(newData(QByteArray)),this, SLOT(get232Data(QByteArray)));
        _232t->start();

        if(t != FUNCTIONTEST_ID)
        {
 //         tid=startTimer(1000);
            _232t->startT(_MS_IDLE_);
        }

        _jtype=t;
    }
}

void CTestJob::connect485(int t)
{
    if(!_status485)
    {
        _status485 = 0;
        _485t = 0;

        _pool485.clear();
        _list485.clear();

        _485t = new C485Thread();
        connect(_485t, SIGNAL(newMessage(QString,bool)),this->parent(), SIGNAL(newMessage(QString,bool)));
        connect(_485t, SIGNAL(newData(QByteArray)),this, SLOT(get485Data(QByteArray)));
        _485t->start();
        _485t->startT(_MS_IDLE_);

        PLCCommandFrame_0x80 c={0};
        initFrame80(&c);
        _jtype=t;

        QByteArray tmp=QByteArray::fromRawData((const char*)&c,sizeof(c));
        setupSendFrame485(tmp,_MS_300_);
    }
}

void CTestJob::start232T(int ms)
{
    if(_232t)
    {
        if(_jtype == FUNCTIONTEST_ID)
        {
            MeterAskFrame_0x16 b;
            b.cmd = _METER_FRAME_METER_PARA_ASK_COMMAND_;
            setupSendFrame232(data16FromFrame(b),_MS_1000_);
        }
        _232t->startT(ms);
    }
}

void CTestJob::stop232T()
{
    if(_232t)
    {
        _232t->stop();
    }
}

void CTestJob::start485T(int ms)
{
    if(_485t)
    {
        _485t->startT(ms);
    }
}

void CTestJob::stop485T()
{
    if(_485t)
    {
        _485t->stop();
    }
}
void CTestJob::get232Data(QByteArray d)
{
    //    QMutexLocker locker(&_mutex1);
    //***转义字处理
    for(int i=0;i<d.size()-1;i++)
    {
        if(d[i]==_DLE_BYTE_ && d[i+1]==_DLE_BYTE_)
            d.remove(i,1);
    }
    _pool232.append(d);

    //***构建帧
    int start=-1,end=-1,i;
    int size=_pool232.size();

    while(size>0)
    {
        if(_pool232[0]==_DLE_BYTE_)
        {
            if(size==1)
                break;
            else if(_pool232[1]==_STX_BYTE_)
            {
                start = 0;
                for(int j=2;j<size-1;j++)
                {
                    if(_pool232[j]==_DLE_BYTE_ && _pool232[j+1]==_ETX_BYTE_)
                    {
                        end = j+1;
                        if(end-start>=5)
                        {
                            QByteArray t1 = _pool232.mid(start,end-start+1);
                            _list232.push_back(t1);
                        }
                        _pool232.remove(start,end-start+1);
                        size = _pool232.size();
                        break;
                    }
                }
                if(end==-1)
                {
                    break;
                }
                else
                {
                    start = -1;
                    end = -1;
                }
            }
            else
            {
                _pool232.remove(0,2);
                size=_pool232.size();
            }
        }
        else
        {
            _pool232.remove(0,1);
            size=_pool232.size();
        }
    }

    //***
    CDataFrame r,s;
    for(int i=0;i<_list232.size();i++)
    {
        CDataFrame r;
        if(!r.inputData(_list232[i]))
        {
            //emit newMessage(tr("接收到一个错误包，丢弃！"),0);
        }
        else
        {
            QString s="";
            for(int j=0;j<_list232[i].size();j++)
            {
                qint8 chr=_list232[i][j];
                s += QString("%1 ").arg(chr&0xFF,2,16,QLatin1Char('0'));
            }

            processFrameData232(r);
        }
        //***删除该帧
        _list232.remove(i);
    }
}


void CTestJob::get485Data(QByteArray d)
{
    //    QMutexLocker locker(&_mutex2);
    //***转义字处理
    for(int i=0;i<d.size()-1;i++)
    {
        if(d[i]==_DLE_BYTE_ && d[i+1]==_DLE_BYTE_)
            d.remove(i,1);
    }
    _pool485.append(d);

    //***构建帧
    int start=-1,end=-1,i;
    int size=_pool485.size();

    while(size>0)
    {
        if(_pool485[0]==_DLE_BYTE_)
        {
            if(size==1)
                break;
            else if(_pool485[1]==_STX_BYTE_)
            {
                start = 0;
                for(int j=2;j<size-1;j++)
                {
                    if(_pool485[j]==_DLE_BYTE_ && _pool485[j+1]==_ETX_BYTE_)
                    {
                        end = j+1;
                        if(end-start>=5)
                        {
                            QByteArray t1 = _pool485.mid(start,end-start+1);
                            _list485.push_back(t1);
                        }
                        _pool485.remove(start,end-start+1);
                        size = _pool485.size();
                        break;
                    }
                }
                if(end==-1)
                {
                    break;
                }
                else
                {
                    start = -1;
                    end = -1;
                }
            }
            else
            {
                _pool485.remove(0,2);
                size=_pool485.size();
            }
        }
        else
        {
            _pool485.remove(0,1);
            size=_pool485.size();
        }
    }

    //***
    CDataFrame r,s;
    for(int i=0;i<_list485.size();i++)
    {
        CDataFrame r;
        if(!r.inputData(_list485[i]))
            emit newMessage(tr("接收到一个错误包，丢弃！"),0);
        else
        {
            QString s="";
            for(int j=0;j<_list485[i].size();j++)
            {
                qint8 chr=_list485[i][j];
                s += QString("%1 ").arg(chr&0xFF,2,16,QLatin1Char('0'));
            }

            processFrameData485(r);
        }
        //***删除该帧
        _list485.remove(i);
    }
}

void CTestJob::processFrameData232(CDataFrame r)
{
    _232tag = QTime::currentTime().secsTo(QTime(2012,1,1));

    QByteArray d;

    switch(r._command)
    {
    case _METER_FRAME_REQUEST_:
        emit newMessage(tr("收到仪表请求帧"),0);
        d[0] = _METER_FRAME_MODE_SETTING_;
        if(_jtype== FULLTEST_ID)
        {
            d[1] = _METER_JOB_MODE_FULLTEST_;
            setupSendFrame232(d,_MS_10_);
        }
        else if(_jtype== ADJUSTTEST_ID)
        {
            d[1] = _METER_JOB_MODE_ADJUSTTEST_;
            setupSendFrame232(d,_MS_10_);
        }

        if(_status232==0 && _jtype==ADJUSTTEST_ID)
        {
            QTimer::singleShot(_MS_500_, this, SLOT(delaySetupAdjustTest()));
        }
        if(_status232==0 && _jtype==FUNCTIONTEST_ID)
        {
            emit _232SetupFunctionTest();
            _status232=1;
        }
        break;
    case _METER_FRAME_HRESPONSE_TEST_:
        if(_status232==0 && _jtype==FULLTEST_ID)
        {
            emit newMessage(tr("收到仪表硬件讯息返回帧"),0);
            hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
            setupSendFrame232(data30FromFrame(b),_MS_200_);
            emit _232SetupFullTest();
            _status232=1;
        }
        if(_jtype==FULLTEST_ID)
            sendBackTestData232(r);
        break;

    case _METER_FRAME_INIT_PRESPONSE_ADJUST_:
        if(_status232==0 && _jtype==ADJUSTTEST_ID)
        {
            emit newMessage(tr("收到仪表调校讯息返回帧"),0);
            emit _232SetupAdjustTest();
            _status232=1;
        }
        if(_jtype==ADJUSTTEST_ID)
            sendBackAdjustData232(r);
        break;

    case _METER_FRAME_CRESPONSE_ADJUST_:
    case _METER_FRAME_RSAVE_PARA_SPEED_ADJUST_:
    case _METER_FRAME_RSAVE_PARA_RSPEED_ADJUST_:
    case _METER_FRAME_RSAVE_PARA_OIL_ADJUST_:
    case _METER_FRAME_RSAVE_PARA_WTEMP_ADJUST_:
    case _METER_FRAME_RSAVE_PARA_CURRENT_ADJUST_:
    case _METER_FRAME_RSAVE_PARA_RESISTOR_ADJUST_:
    case _METER_FRAME_RSAVE_PARA_VOLTAGE_ADJUST_:
    case _METER_FRAME_RSAVE_PARA_POWER_ADJUST_:
    case _METER_FRAME_RSAVE_ADJUST_:
        if(_jtype==ADJUSTTEST_ID)
            sendBackAdjustData232(r);

    case _METER_FRAME_METER_PARA_RASK_COMMAND_:
        sendBackFunctionData232(r);

    case _METER_FRAME_METER_PARA_RSAVE_COMMAND_:
        sendBackFunctionData232(r);

    default:
        break;
    }
}

void CTestJob::delaySetupAdjustTest()
{
    adjustInitParaFrame_0x41 b={_METER_FRAME_INIT_PARA_ADJUST_,0x00};
    QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
    setupSendFrame232(tmp,_MS_300_);
}

void CTestJob::processFrameData485(CDataFrame r)
{
    _485tag = QTime::currentTime().secsTo(QTime(2012,1,1));

    QByteArray d;
    switch(r._command)
    {
    case _PLC_FRAME_CRESPONSE_:
        if(_status485==0)
        {
            emit newMessage(tr("收到信号箱反馈帧"),0);
            if(_jtype == FULLTEST_ID)
                emit _485SetupFullTest();
            else if(_jtype == ADJUSTTEST_ID)
                emit _485SetupAdjustTest();
            else if(_jtype == FUNCTIONTEST_ID)
                emit _485SetupFunctionTest();

            _status485=1;
        }
        sendBackTestData485(r);
        break;

    default:
        break;
    }
}

void CTestJob::setupSendFrame232(QByteArray d,int ms)
{
    if(_232t!=0)
    {
        CDataFrame df;
        df.setupDataFrame(d);
        _232t->setSendPackage(df.formatOutputDataStream());
        _232t->changeInterval(ms);
    }
}

void CTestJob::setupSendFrame485(QByteArray d,int ms)
{
    if(_485t!=0)
    {
        CDataFrame df;
        df.setupDataFrame(d);
        _485t->setSendPackage(df.formatOutputDataStream());
        _485t->changeInterval(ms);
    }
}

void CTestJob::sendTestData232(QByteArray a)
{
    if(_232t!=0)
    {
        CDataFrame df;
        df.setupDataFrame(a);
        if(_jtype==FULLTEST_ID)
            _232t->changeInterval(_MS_200_);
        else if(_jtype==ADJUSTTEST_ID)
            _232t->changeInterval(_MS_300_);

        _232t->setSendPackage(df.formatOutputDataStream());
    }
}

void CTestJob::sendTestData485(QByteArray a)
{
    if(_485t!=0)
    {
        CDataFrame df;
        df.setupDataFrame(a);
        _485t->setSendPackage(df.formatOutputDataStream());
    }
}

void CTestJob::sendBackTestData232(CDataFrame a)
{
    QByteArray b = a.getData();
    switch(_ttype)
    {
    case FULL_TEST_KEY_ID:
        emit sendBackFullTestData_key(b);
        break;
    case FULL_TEST_LED_ID:
        emit sendBackFullTestData_led(b);
        break;
    case FULL_TEST_LCD_BACKLIGHT_ID:
        emit sendBackFullTestData_lcdbacklight(b);
        break;
    case FULL_TEST_IO_IN_ID:
        emit sendBackFullTestData_ioi_232(b);
        break;
    case FULL_TEST_IO_OUT_ID:
        emit sendBackFullTestData_ioo_232(b);
        break;
    case FULL_TEST_FREQ_ID:
        emit sendBackFullTestData_freq_232(b);
        break;
    case FULL_TEST_ANALOG_ID:
        emit sendBackFullTestData_analog_232(b);
        break;
    case FULL_TEST_MEMORY_ID:
        emit sendBackFullTestData_memory(b);
        break;
    case FULL_TEST_CAN_ID:
        emit sendBackFullTestData_can_232(b);
        break;
    case FULL_TEST_SELF_POWER_ID:
        emit sendBackFullTestData_selfpower(b);
        break;
    case FULL_TEST_METER_VOLTAGE_ID:
        emit sendBackFullTestData_metervoltage232(b);
        break;
    }
}

void CTestJob::sendBackTestData485(CDataFrame a)
{
    QByteArray b = a.getData();
    switch(_ttype)
    {
    case FULL_TEST_IO_IN_ID:
        emit sendBackFullTestData_ioi_485(b);
        break;
    case FULL_TEST_FREQ_ID:
        emit sendBackFullTestData_freq_485(b);
        break;
    case FULL_TEST_IO_OUT_ID:
        emit sendBackFullTestData_ioo_485(b);
        break;
    case FULL_TEST_ANALOG_ID:
        emit sendBackFullTestData_analog_485(b);
        break;
    case FULL_TEST_METER_VOLTAGE_ID:
        emit sendBackFullTestData_metervoltage485(b);
        break;
    case FULL_TEST_CAN_ID:
        emit sendBackFullTestData_can_485(b);
        break;
    }
}

void CTestJob::sendBackAdjustData232(CDataFrame a)
{
    switch(_ttype)
    {
    case ADJUST_TEST_SPEED_ID:
        emit sendBackAdjustTestData_speed(a);
        break;
    case ADJUST_TEST_RSPEED_ID:
        emit sendBackAdjustTestData_rotate(a);
        break;
    case ADJUST_TEST_WATERTEMP_ID:
        emit sendBackAdjustTestData_watertemp(a);
    case ADJUST_TEST_OIL_ID:
        emit sendBackAdjustTestData_oil(a);
    case ADJUST_TEST_ANALOG_ID:
        emit sendBackAdjustTestData_analog_232(a);
    }
}

void CTestJob::sendBackAdjustData485(CDataFrame a)
{
    switch(_ttype)
    {
    case ADJUST_TEST_ANALOG_ID:
        emit sendBackAdjustTestData_analog_485(a.getData());
    }
}

void CTestJob::sendBackFunctionData232(CDataFrame a)
{
    emit sendBackFunctionTestData_232(a);
}


void CTestJob::initFrame80(PLCCommandFrame_0x80* a)
{
    a->cmd =  _PLC_FRAME_COMMAND_;
    a->id = 0;
    a->_12v = 0;    //* 没有常电
    a->_xxv = 2;    //* 24V
    //***IO输入

    for(int i=0;i<10;i++)
        a->ioo[i] = 0xFF;
/*
    for(int i=0;i<_mconfig->_map_ioin.size();i++)
    {
        int j = _mconfig->_map_ioin[i];
        int d = (int)(j/4);
        int r = j - d*4;

        a->ioo[d] &= ~(3<<(2*r));
        if(getIOInMapVoltage(i))
        {
            a->ioo[d] |= 1<<(2*r);
        }
        else
        {
            a->ioo[d] |= 2<<(2*r);
        }

        //***差分IO
        int kk = getIOInMapDifference(i);
        if(kk!=-1)
        {
            int n = (int)(kk/4);
            int o = kk - n*4;

            a->ioo[n] &= ~(3<<(2*o));
            a->ioo[n] |= 1<<(2*o);
        }
    }
*/
    //***功能全检状态下，置高仪表ACC和ON开关量
    if(_jtype == FUNCTIONTEST_ID)
    {
        int jj = _mconfig->_map_ioin[12];
        int d = (int)(jj/4);
        int r = jj - d*4;

        a->ioo[d] &= ~(3<<(2*r));
        a->ioo[d] |= 2<<(2*r);

        jj = _mconfig->_map_ioin[16];
        d = (int)(jj/4);
        r = jj - d*4;

        a->ioo[d] &= ~(3<<(2*r));
        a->ioo[d] |= 2<<(2*r);
    }
}

bool CTestJob::getIOInMapVoltage(quint8 index)
{
    return getTestBit(_mconfig->_map_ioin_def[index],0);
}

int CTestJob::getIOInMapCurrent(quint8 index)
{
    int m=0,n=-1;
    for(int i=0;i<_mconfig->_map_ioin_def.size();i++)
    {
        if(getTestBit(_mconfig->_map_ioin_def[i],1))
        {
            if(i==index)
            {
                n = _mconfig->_map_cfb_voltage[m];

                return n;
            }
            m++;
        }
    }
    return n;
}

int CTestJob::getIOInMapCurrentChannel(int index)
{
    int m=0;
    for(int i=0;i<_mconfig->_map_ioin_def.size();i++)
    {
        if(getTestBit(_mconfig->_map_ioin_def[i],1))
        {
            if(m==index)
                return i;

            m++;
        }
    }

    return -1;
}

int CTestJob::getIOInMapDifference(quint8 index)
{
    int m=0,n=-1;
    for(int i=0;i<_mconfig->_map_ioin_def.size();i++)
    {
        if(getTestBit(_mconfig->_map_ioin_def[i],2))
        {
            if(i==index)
            {
                n = _mconfig->_map_ioin_dif_neg[m];

                return n;
            }
            m++;
        }
    }
    return n;
}


