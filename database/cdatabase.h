#ifndef CDATABASE_H
#define CDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include <QSqlIndex>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDateTime>

#include <QVector>
#include "protocol/protocol.h"

class CDatabase : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase    _db;

public:
    explicit CDatabase(QString db,QObject *parent = 0);
    
signals:
    
public slots:
    bool        Open();
    QString     lastError();
    QString     execSQL(QString);
    qint32      validateUser(QString u,QString p);
    bool        getMeter(QString sid);
    bool        insertMeter(QString,QString,QDateTime);
    bool        insertTest(QString sid,QString user,QDateTime t);
    bool        deleteMeter(QString id);
    QVector<testRecord> getTest(QString);
};

#endif // CDATABASE_H
