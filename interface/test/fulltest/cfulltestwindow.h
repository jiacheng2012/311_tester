#ifndef CFULLTESTWINDOW_H
#define CFULLTESTWINDOW_H

#include <QWidget>
#include <QtGui>

#include "cfulltestoverviewwindow.h"
#include "cfulltestiooutwindow.h"
#include "cfulltestioinwindow.h"
#include "cfulltestanalogwindow.h"
#include "cfulltestcanwindow.h"
#include "cfulltestfreqwindow.h"
#include "cfulltestkeywindow.h"
#include "cfulltestlcdbacklightwindow.h"
#include "cfulltestlcdwindow.h"
#include "cfulltestledwindow.h"
#include "cfulltestmemorywindow.h"
#include "cfulltestmotorwindow.h"
#include "cfulltestselfpowerwindow.h"
#include "cfulltestmetervoltagewindow.h"
#include "../../cobjectfactory.h"

class CFullTestWindow : public QWidget
{
    Q_OBJECT

public:
    CObjectFactory              _factory;
    QMap<QString, int>         _objectMap;
    QListWidget                 *_listLayout;
    QHBoxLayout                 *_topLayout;

public:
    Q_INVOKABLE CFullTestWindow(QWidget *parent = 0);

signals:
    void newMessage(QString,bool);

public slots:
    void listLayoutClicked(QModelIndex);
    void nextList(int);
    void delayClick();

};

#endif // CFULLTESTWINDOW_H
