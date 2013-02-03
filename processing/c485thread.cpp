#include <QtDebug>
#include "C485Thread.h"

C485Thread::C485Thread()
{
    //***启动定时器
    _timer = new QTimer();
    connect(_timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
}

//***Destructor
C485Thread::~C485Thread()
{
    _port->Close();
    _timer->stop();
    delete _timer;
}

void C485Thread::startT(int ms)
{
    _timer->start(ms);
}

void C485Thread::stopT()
{
    _timer->stop();
}

void C485Thread::changeInterval(int ms)
{
    _timer->stop();
    _timer->start(ms);
}

void C485Thread::setSendPackage(QByteArray d)
{
    _sendMsg = d;
}

void C485Thread::timerUpdate()
{
    _port->Send(_sendMsg);
}

//***Run
void C485Thread::run()
{
//  _timer->moveToThread(this);
    _port = new C485Port();
    connect(_port,SIGNAL(newData(QByteArray)),this,SLOT(getData(QByteArray)));
    connect(_port,SIGNAL(newMessage(QString,bool)),this,SIGNAL(newMessage(QString,bool)));

    emit newMessage("启动485串口线程",0);
    //***串口设置
    _port->Open();

    exec();
}

void C485Thread::stop()
{
    emit newMessage("结束线程",0);
}

void C485Thread::getData(QByteArray data)
{
 //   emit newMessage(QString::fromAscii(data.data()),0);
    emit newData(data);
}
