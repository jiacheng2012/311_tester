#include "c485port.h"
#include "cdataframe.h"
#include "capp.h"

C485Port::C485Port()
{
    //***设置串口类
    CConfig *cf = ((CApp*)qApp)->_config;

    //***设置串口类
    _port = new QextSerialPort("COM" + QString::number(cf->_connectParmsMap["plc-portnumber"]+1), QextSerialPort::EventDriven);
/*
    _port = new SerialPort;

    foreach (const SerialPortInfo &info, SerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        if(info.portName()=="COM4")
            _info = info;
    }*/
}

C485Port::~C485Port()
{
    _port = 0;
}

void C485Port::Open()
{/*
    //***打开串口
    _port->setPort(_info);
    if(!_port->open(QIODevice::ReadWrite))
        qDebug() << "open error";
    if(!_port->setRate(SerialPort::Rate57600))
        qDebug() << "rate error";
    if(!_port->setDataBits(SerialPort::Data8))
        qDebug() << "bit error";
    if(!_port->setFlowControl(SerialPort::NoFlowControl))
        qDebug() << "flow error";
    if(!_port->setParity(SerialPort::NoParity))
        qDebug() << "parity error";
    if(!_port->setStopBits(SerialPort::OneStop))
        qDebug() << "stop error";
*/
    CConfig *cf = ((CApp*)qApp)->_config;
    _port->open(QIODevice::ReadWrite);
    switch(cf->_connectParmsMap["meter-baudrate"])
    {
    case 0:
        _port->setBaudRate(BAUD9600);
        break;
    case 1:
        _port->setBaudRate(BAUD19200);
        break;
    case 2:
        _port->setBaudRate(BAUD38400);
        break;
    case 3:
        _port->setBaudRate(BAUD57600);
        break;
    case 4:
        _port->setBaudRate(BAUD115200);
        break;
    }
    switch(cf->_connectParmsMap["meter-databit"])
    {
    case 0:
        _port->setDataBits(DATA_5);
        break;
    case 1:
        _port->setDataBits(DATA_6);
        break;
    case 2:
        _port->setDataBits(DATA_7);
        break;
    case 3:
        _port->setDataBits(DATA_8);
        break;
    }
    switch(cf->_connectParmsMap["meter-stopbit"])
    {
    case 0:
        _port->setStopBits(STOP_1);
        break;
    case 1:
        _port->setStopBits(STOP_1_5);
        break;
    case 2:
        _port->setStopBits(STOP_2);
        break;
    }
    switch(cf->_connectParmsMap["meter-dataflowcontrol"])
    {
    case 0:
        _port->setFlowControl(FLOW_OFF);
        break;
    case 1:
        _port->setFlowControl(FLOW_HARDWARE);
        break;
    case 2:
        _port->setFlowControl(FLOW_XONXOFF);
        break;
    }
    switch(cf->_connectParmsMap["meter-paritycheck"])
    {
    case 0:
        _port->setParity(PAR_NONE);
        break;
    case 1:
        _port->setParity(PAR_ODD);
        break;
    case 2:
        _port->setParity(PAR_EVEN);
        break;
    case 3:
        _port->setParity(PAR_MARK);
        break;
    case 4:
        _port->setParity(PAR_SPACE);
        break;
    }
    _port->setQueryMode(QextSerialPort::EventDriven);

    //设置过期时间为1000 ms
    _port->setTimeout(100);

    //***接受数据
    connect(_port, SIGNAL(readyRead()), this, SLOT(Receive()));
    qDebug("is open: %d", _port->isOpen());
}

void C485Port::Close()
{
    _port->close();
    qDebug("is open: %d", _port->isOpen());
}

void C485Port::Send(QByteArray data)
{
    int i = _port->write(data);
}

void C485Port::Receive()
{
    char buff[1024];
    int numBytes = _port->bytesAvailable();
    if(numBytes>1024)
        numBytes = 1024;
//  _port->read(buff,numBytes);
//  QByteArray tmp(buff);
    QByteArray tmp=_port->readAll();
    emit newData(tmp);
}
