#ifndef C485THREAD_H
#define C485THREAD_H

#include <QThread>
#include <QtNetwork>
#include "protocol/c485port.h"

class C485Thread : public QThread
{
 Q_OBJECT

 public:
   C485Thread();

 protected:
   ~C485Thread();

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
    C485Port         *_port;
    QTimer           *_timer;
    QMutex           _mutex;
    QByteArray       _sendMsg,_receiveMsg;

public slots:
    void            timerUpdate();
    void            getData(QByteArray);
    void            setSendPackage(QByteArray);
};

#endif // C485THREAD_H
