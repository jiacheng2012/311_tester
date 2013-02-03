#ifndef CADJUSTTESTWINDOW_H
#define CADJUSTTESTWINDOW_H

#include <QWidget>
#include "cadjusttestoverviewwindow.h"
#include "cadjusttestspeedtablewindow.h"
#include "cadjusttestrotatetablewindow.h"
#include "cadjusttestoiltablewindow.h"
#include "cadjusttestwatertemperaturewindow.h"
#include "cadjusttestanalogwindow.h"
#include "../../cobjectfactory.h"

class CAdjustTestWindow : public QWidget
{
    Q_OBJECT

public:
    CObjectFactory              _factory;
    QMap<QString, int>         _objectMap;
    QListWidget                 *_listLayout;
    QHBoxLayout                 *_topLayout;
public:
    Q_INVOKABLE CAdjustTestWindow(QWidget *parent = 0);
signals:
    void newMessage(QString,bool);

public slots:
    void listLayoutClicked(QModelIndex);
    void nextList(int);
    void delayClick();
};

#endif
