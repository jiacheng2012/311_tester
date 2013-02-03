#ifndef CDATAFRAME_H
#define CDATAFRAME_H

#include <QObject>
#include <QByteArray>
#include "protocol.h"

class CDataFrame
{
public:
    CDataFrame();

public:
    QByteArray  _data;
    quint8      _crc;
    quint8      _command;
    QByteArray  _data_stream;

public:
    bool        setupDataFrame(QByteArray &data);
    QByteArray  formatOutputDataStream();
    quint8      CRC8_Get(quint8* buf, quint8 len);
    quint8      getSize();
    bool        inputData(QByteArray);
    quint8      getType();
    QByteArray  getData();

};

#endif
