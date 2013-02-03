#ifndef CFULLTESTOVERVIEWWINDOW_H
#define CFULLTESTOVERVIEWWINDOW_H

#include <QtGui>
#include <QWidget>
#include "configure/cmeterconfig.h"

class CFullTestOverviewWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFullTestOverviewWindow(QWidget *parent = 0);

    QPushButton *_232Button,*_485Button,*_nextButton;
    
    int     _connected;

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void _232ButtonClicked();
    void _485ButtonClicked();
    void _232Setup();
    void _485Setup();
    void nextButtonClicked();
    void stop232();
    void stop485();
};

#endif
