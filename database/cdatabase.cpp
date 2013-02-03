#include "cdatabase.h"
#include <QDebug>

CDatabase::CDatabase(QString db,QObject *parent) :
    QObject(parent)
{
    _db = QSqlDatabase::addDatabase("QODBC");
    _db.setDatabaseName(QString("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=") + db);
}

bool CDatabase::Open()
{
    bool ok = _db.open();

    return ok;
}

QString CDatabase::lastError()
{
    return _db.lastError().text();
}

QString CDatabase::execSQL(QString q)
{
    QSqlQuery   sql(_db);
    sql.exec(q);

    return _db.lastError().text();
}

//***数据库检索功能
//***用户验证
qint32 CDatabase::validateUser(QString u,QString p)
{
    QSqlQuery sql(_db);
    sql.exec("select type from user where user='" + u + "' and passwd='" + p + "'");
    if(sql.next())
        return sql.value(0).toInt();
    else
        return -1;
}

//***检索仪表检测结果
bool CDatabase::getMeter(QString sid)
{
    QSqlQuery sql(_db);
    sql.exec("select * from meter where sid='" + sid +"'");
    if(sql.next())
        return true;
    else
        return false;
}

//***插入新的仪表纪录
bool CDatabase::insertMeter(QString id,QString user,QDateTime t)
{
    QSqlQuery sql(_db);
    if (!sql.exec("insert into meter (sid,cdate,cuser) values ('"+ id + "'," + QString::number(t.toTime_t())+ ",'"+user+"')"))
    {
        qDebug() << sql.lastError();
        return false;
    }
    else
        return true;
}

//***插入新的仪表纪录
bool CDatabase::insertTest(QString sid,QString user,QDateTime t)
{
    QSqlQuery sql(_db);
    if (!sql.exec("insert into test (meter,cdate,user) values ('"+ sid + "'," + QString::number(t.toTime_t())+ ",'"+user+"')"))
    {
        qDebug() << sql.lastError();
        return false;
    }
    else
        return true;
}

//***提取测试记录
QVector<testRecord> CDatabase::getTest(QString id)
{
    QVector<testRecord> t;

    QSqlQuery sql(_db);
    sql.exec("select * from test where meter='" + id + "'");
    int i=0;
    while(sql.next())
    {
        testRecord a;
        a._id       = sql.value(0).toInt();
        a._meter    = sql.value(1).toString();
        a._user     = sql.value(2).toString();
        a._date     = sql.value(3).toInt();
        a._inittest = sql.value(4).toInt();
        a._fulltest = sql.value(5).toInt();
        a._adjusttest   = sql.value(6).toInt();
        a._functiontest = sql.value(7).toInt();
        a._comment      = sql.value(8).toString();

        t.insert(i++,1,a);
    }

    return t;
}
//***2013.1.27 删除最近本次的测试记录
bool CDatabase::deleteMeter(QString id)
{
    QSqlQuery sql(_db);
    return sql.exec("delete * from test where ID= " + id );
}
