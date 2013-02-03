#ifndef C232THREAD_H
#define C232THREAD_H

#include <QThread>
#include <QtNetwork>
#include "protocol/c232port.h"

class C232Thread : public QThread
{
 Q_OBJECT

 public:
   C232Thread();

 protected:
   ~C232Thread();

 public:
   void initialize();


 signals:
    void newMessage(QString,bool);
    void newData(QByteArray);
 public:
    virtual void run();
    void    stop();
    void    changeInterval(int);
    void    startT(int ms);
    void    stopT();

 public:
    C232Port         *_port;
    QTimer           *_timer;
    QMutex           _mutex;
    QByteArray       _sendMsg,_receiveMsg;

public slots:
    void            timerUpdate();
    void            getData(QByteArray);
    void            setSendPackage(QByteArray);
};

#endif // C232Thread_H
