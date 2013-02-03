#ifndef CADJUSTCONFIG_H
#define CADJUSTCONFIG_H

#include <QObject>
#include "protocol/protocol.h"

class CAdjustConfig : public QObject
{
    Q_OBJECT
public:
    explicit CAdjustConfig(QObject *parent = 0);
    
    quint32 readFile(QString fl);
    quint32 writeFile(QString f1);

    //***调校参数表: 93bytes
    quint32                     date;
    quint8                      version;
    quint8                      version1;

    QMap<QString,vu16>          value_meter;
    QMap<QString,v16>           sample_meter;
    QMap<int,quint8>            type_ana;
    QMap<int,vu16>              value_ana;
    QMap<int,vu16>              sample_ana;
    quint8                      oil_rcof;

signals:
    
public slots:
    
};

#endif
