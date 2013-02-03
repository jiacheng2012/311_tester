#ifndef C232PORT_H
#define C232PORT_H

#include <QObject>
#include "qextserialport.h"
#include "cdataframe.h"
//#include <QtAddonSerialPort/serialport.h>
//#include <QtAddonSerialPort/serialportinfo.h>

//QT_USE_NAMESPACE_SERIALPORT

class C232Port : public QObject
{
    Q_OBJECT
public:
    C232Port();
    ~C232Port();

    void Open();
    void Close();
    void Send(QByteArray);
    void parseData();
    void appendCR(QString);
    void appendLF(QString);


public:
    QextSerialPort  *_port;
//  SerialPort      *_port;
    QByteArray      *_buffer;
//  SerialPortInfo  _info;

signals:
    void newData(QByteArray);
    void newMessage(QString,bool);

public slots:
    void Receive();
};

#endif
