#ifndef CFUNCTIONTESTWINDOW_H
#define CFUNCTIONTESTWINDOW_H

#include <QWidget>
#include <QtGui>
#include "cfunctiontestoverviewwindow.h"
#include "cfunctiontestledwindow.h"
#include "cfunctiontestspeedtablewindow.h"
#include "cfunctiontestrotatetablewindow.h"
#include "cfunctiontestfueloiltablewindow.h"
#include "cfunctiontestwatertemperaturewindow.h"
#include "cfunctiontestpressurewindow.h"
#include "cfunctiontestoilpressurewindow.h"
#include "cfunctiontestinlettemperaturewindow.h"
#include "cfunctiontestvehiclevoltagewindow.h"
#include "cfunctiontestsettingwindow.h"
#include "../../cobjectfactory.h"

class CFunctionTestWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFunctionTestWindow(QWidget *parent = 0);
public:
    CObjectFactory              _factory;
    QMap<QString, int>         _objectMap;
    QListWidget                 *_listLayout;
    QHBoxLayout                 *_topLayout;
signals:
    void newMessage(QString,bool);
    
public slots:
    void listLayoutClicked(QModelIndex);
    void nextList(int);
    void delayClick();

};

#endif // CFUNCTIONTESTWINDOW_H
