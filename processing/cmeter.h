#ifndef CMETER_H
#define CMETER_H

#include <QObject>
#include <QDate>
#include <QTime>
#include "protocol/protocol.h"
#include <QVector>

class CMeter : public QObject
{
    Q_OBJECT
public:
    CMeter(QString);

public:
    QString             _sid;
    QDate               _start,_end;
    bool                _status;
    QVector<testRecord> _tr;
    testRecord          _ctr;

signals:
    
public slots:
    
};

#endif // CMETER_H
