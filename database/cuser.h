#ifndef CUSER_H
#define CUSER_H

#include <QObject>

class CUser
{

public:
    QString     _user;
    quint32     _level;
    quint32     _id;
    QString     _name;

public:
    CUser();

signals:
    
public slots:
    
};

#endif // CUSER_H
