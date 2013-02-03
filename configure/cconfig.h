#ifndef CCONFIG_H
#define CCONFIG_H

#include <QObject>
#include <QMap>
#include "protocol/protocol.h"

class CConfig : public QObject
{
    Q_OBJECT

public:
    explicit CConfig(QObject *parent = 0);
    quint32 readFile(QString fl);

    QString             _dbName;
    QMap<QString,int>   _connectParmsMap;//ini配置文件中PLC,仪表,CAN连接设置 键值对
    QString             _mfile,_afile;
    int                 current_fb_resistor;
    int                 load0,load1,load2,load3,load4,load5;
    
signals:
    
public slots:
    

};

#endif // CCONFIG_H
