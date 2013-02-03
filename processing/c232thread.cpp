#include <QtDebug>
#include "C232Thread.h"

C232Thread::C232Thread()
{
    //***启动定时器
    _timer = new QTimer();
    connect(_timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
}

//***Destructor
C232Thread::~C232Thread()
{
    _port->Close();
    _timer->stop();
}

void C232Thread::startT(int ms)
{
    _timer->start(ms);
}

void C232Thread::stopT()
{
    _timer->stop();
}

void C232Thread::changeInterval(int ms)
{
    _timer->stop();
    _timer->start(ms);
}

void C232Thread::setSendPackage(QByteArray d)
{
    _sendMsg = d;
}

void C232Thread::timerUpdate()
{
    _port->Send(_sendMsg);
}

//***Run
void C232Thread::run()
{
//  _timer->moveToThread(this);
    _port = new C232Port();
    connect(_port,SIGNAL(newData(QByteArray)),this,SLOT(getData(QByteArray)));
    connect(_port,SIGNAL(newMessage(QString,bool)),this,SIGNAL(newMessage(QString,bool)));

    emit newMessage("启动232串口线程",0);
    //***串口设置
    _port->Open();

    exec();
}

void C232Thread::stop()
{
    emit newMessage("结束线程",0);
}

void C232Thread::getData(QByteArray data)
{
 //   emit newMessage(QString::fromAscii(data.data()),0);
    emit newData(data);
}
