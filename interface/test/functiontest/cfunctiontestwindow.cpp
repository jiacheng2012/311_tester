#include "cfunctiontestwindow.h"
#include "capp.h"
#include <QTimer>

CFunctionTestWindow::CFunctionTestWindow(QWidget *parent) :
    QWidget(parent)
{
    //***UI Component
    _listLayout = new QListWidget;
    _listLayout->addItem(tr("测试介绍"));
    _listLayout->addItem(tr("下载应用参数"));
//    _listLayout->addItem(tr("指示灯测试"));
    _listLayout->addItem(tr("车速表测试"));
    _listLayout->addItem(tr("转速表测试"));
    _listLayout->addItem(tr("燃油表测试"));
    _listLayout->addItem(tr("水温表测试"));
    _listLayout->addItem(tr("气压测试"));
    _listLayout->addItem(tr("机油压力测试"));
    _listLayout->addItem(tr("进气温度测试"));
    _listLayout->addItem(tr("整车电压测试"));
    _listLayout->setFixedWidth(140);
    _listLayout->setFixedHeight(300);
    _listLayout->setProperty("li-row", 0);

    _factory.registerObject<CFunctionTestOverviewWindow>("CFunctionTestOverviewWindow");
    _factory.registerObject<CFunctionTestSettingWindow>("CFunctionTestSettingWindow");
    _factory.registerObject<CFunctionTestLEDWindow>("CFunctionTestLEDWindow");
    _factory.registerObject<CFunctionTestSpeedTableWindow>("CFunctionTestSpeedTableWindow");
    _factory.registerObject<CFunctionTestRotateTableWindow>("CFunctionTestRotateTableWindow");
    _factory.registerObject<CFunctionTestFuelOilTableWindow>("CFunctionTestFuelOilTableWindow");
    _factory.registerObject<CFunctionTestWaterTemperatureWindow>("CFunctionTestWaterTemperatureWindow");
    _factory.registerObject<CFunctionTestPressureWindow>("CFunctionTestPressureWindow");
    _factory.registerObject<CFunctionTestOilPressureWindow>("CFunctionTestOilPressureWindow");
    _factory.registerObject<CFunctionTestInletTemperatureWindow>("CFunctionTestInletTemperatureWindow");
    _factory.registerObject<CFunctionTestVeHicleVoltageWindow>("CFunctionTestVeHicleVoltageWindow");

    _objectMap.clear();
    QStringList classNames;
    classNames << "CFunctionTestOverviewWindow" << "CFunctionTestSettingWindow" /*<< "CFunctionTestLEDWindow"*/
               << "CFunctionTestSpeedTableWindow"<< "CFunctionTestRotateTableWindow"<< "CFunctionTestFuelOilTableWindow"
               << "CFunctionTestWaterTemperatureWindow" << "CFunctionTestPressureWindow" << "CFunctionTestOilPressureWindow"
               << "CFunctionTestInletTemperatureWindow" << "CFunctionTestVeHicleVoltageWindow";
    foreach(QString name, classNames)
    {
        _objectMap[name] = classNames.indexOf(name);
    }

    _topLayout = new QHBoxLayout;
    _topLayout->addWidget(_listLayout, 0, Qt::AlignLeft|Qt::AlignTop);
    _topLayout->addWidget(new CFunctionTestOverviewWindow, 0, Qt::AlignLeft|Qt::AlignTop);
    setLayout(_topLayout);

    //***Signal
    connect(_listLayout, SIGNAL(clicked(QModelIndex)),this, SLOT(listLayoutClicked(QModelIndex)));
}

void CFunctionTestWindow::listLayoutClicked(QModelIndex index)
{
    if( index.row() == _listLayout->property("li-row").toInt() )
        return;
    //***判断设备连接状态
    /*if(((CApp*)qApp)->_tjob->_status232 == 0)
    {
       emit newMessage(tr("仪表串口没有连接，请先连接"),1);
       return;
    }
    else if(((CApp*)qApp)->_tjob->_status485 == 0)
    {
       emit newMessage(tr("信号箱没有连接，请先连接"),1);
       return;
    }*/

     MeterAskFrame_0x16 b={_METER_FRAME_METER_PARA_ASK_COMMAND_};
     ((CApp*)qApp)->_tjob->setupSendFrame232(data16FromFrame(b),_MS_50_);

    //***clear 485
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    QByteArray tmp=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->setupSendFrame485(tmp,_MS_50_);
    setProperty("row",index.row());
    QTimer::singleShot(_MS_300_, this, SLOT(delayClick()));
}

void CFunctionTestWindow::delayClick()
{
    int m = property("row").toInt();
    //***删除原来的widget
    QLayoutItem* item = 0;
    QWidget* widget = 0;
    if( (item = _topLayout->itemAt(1)) && (widget = _topLayout->itemAt(1)->widget()) )
    {
        widget->hide();
        delete widget;
    }
    //***新创建 widget
    QWidget *newwidget = _factory.createObject<QWidget>(_objectMap.key(m));
    connect(newwidget,SIGNAL(nextList(int)),this,SLOT(nextList(int)));
    connect(newwidget,SIGNAL(newMessage(QString,bool)),this->parent(),SLOT(newMessage(QString,bool)));

    _topLayout->addWidget(newwidget, 0, Qt::AlignLeft|Qt::AlignTop);
    _listLayout->setProperty("li-row", m);
    ((CApp*)qApp)->_tjob->_ttype = m-1;
}

void CFunctionTestWindow::nextList(int i)
{
    if( i == _listLayout->count() - 3 )
        return;
    //***删除原来的widget
    QLayoutItem* item = 0;
    QWidget* widget = 0;
    if( (item = _topLayout->itemAt(1)) && (widget = _topLayout->itemAt(1)->widget()) )
    {
        widget->hide();
        delete widget;
    }
    //***新创建 widget
    QWidget *newwidget = _factory.createObject<QWidget>(_objectMap.key( i+3 ));

    _topLayout->addWidget(newwidget, 0, Qt::AlignLeft|Qt::AlignTop);
    connect(newwidget,SIGNAL(nextList(int)),this,SLOT(nextList(int)));
    connect(newwidget,SIGNAL(newMessage(QString,bool)),this->parent(),SLOT(newMessage(QString,bool)));

    _listLayout->setCurrentRow( i+3 );
    _listLayout->setProperty("li-row",  i+3 );
    ((CApp*)qApp)->_tjob->_ttype = i + 1;
}
