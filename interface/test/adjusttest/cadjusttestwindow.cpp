#include "cadjusttestwindow.h"
#include "capp.h"
#include <QTimer>

CAdjustTestWindow::CAdjustTestWindow(QWidget *parent) :
    QWidget(parent)
{
    //***UI Component

    _listLayout = new QListWidget;
    _listLayout->addItem(tr("测试介绍"));
    _listLayout->addItem(tr("车速表调校"));
    _listLayout->addItem(tr("转速表调校"));
    _listLayout->addItem(tr("燃油表调校"));
    _listLayout->addItem(tr("水温表调校"));
    _listLayout->addItem(tr("模拟量调校"));
    _listLayout->setFixedWidth(140);
    _listLayout->setFixedHeight(300);
    _listLayout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    _listLayout->setCurrentRow(0);
    _listLayout->setProperty("li-row", 0);

    _factory.registerObject<CAdjustTestOverviewWindow>("CAdjustTestOverviewWindow");
    _factory.registerObject<CAdjustTestSpeedTableWindow>("CAdjustTestSpeedTableWindow");
    _factory.registerObject<CAdjustTestRotateTableWindow>("CAdjustTestRotateTableWindow");
    _factory.registerObject<CAdjustTestOilTableWindow>("CAdjustTestOilTableWindow");
    _factory.registerObject<CAdjustTestWaterTemperatureWindow>("CAdjustTestWaterTemperatureWindow");
    _factory.registerObject<CAdjustTestAnalogWindow>("CAdjustTestAnalogWindow");

    _objectMap.clear();
    QStringList classNames;
    classNames << "CAdjustTestOverviewWindow" << "CAdjustTestSpeedTableWindow"<< "CAdjustTestRotateTableWindow"
               << "CAdjustTestOilTableWindow"<< "CAdjustTestWaterTemperatureWindow"<< "CAdjustTestAnalogWindow";
    foreach(QString name, classNames)
    {
        _objectMap[name] = classNames.indexOf(name);
    }

    _topLayout = new QHBoxLayout;

    _topLayout->addWidget(_listLayout, 0, Qt::AlignLeft|Qt::AlignTop);
    _topLayout->addWidget(new CAdjustTestOverviewWindow, 0, Qt::AlignLeft|Qt::AlignTop);
    _topLayout->setContentsMargins(10, 10, 15, 10);
    setLayout(_topLayout);

    //***Signal
    connect(_listLayout, SIGNAL(clicked(QModelIndex)),this, SLOT(listLayoutClicked(QModelIndex)));
}

void CAdjustTestWindow::listLayoutClicked(QModelIndex index)
{
    if( index.row() == _listLayout->property("li-row").toInt() )
        return;
    //***判断设备连接状态
    if(((CApp*)qApp)->_tjob->_status232 == 0)
    {
        emit newMessage(tr("仪表串口没有连接，请先连接"),1);
        return;
    }
    else if(((CApp*)qApp)->_tjob->_status485 == 0)
    {
        emit newMessage(tr("信号箱没有连接，请先连接"),1);
        return;
    }

    adjustInitParaFrame_0x41 b={_METER_FRAME_INIT_PARA_ADJUST_,0x00};
    QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
    ((CApp*)qApp)->_tjob->setupSendFrame232(tmp,_MS_50_);

    //***clear 485
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    tmp=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->setupSendFrame485(tmp,_MS_50_);
    setProperty("row",index.row());
    QTimer::singleShot(_MS_300_, this, SLOT(delayClick()));
}

void CAdjustTestWindow::delayClick()
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
    //newwidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(newwidget,SIGNAL(nextList(int)),this,SLOT(nextList(int)));
    connect(newwidget,SIGNAL(newMessage(QString,bool)),this->parent(),SLOT(newMessage(QString,bool)));

    _topLayout->addWidget(newwidget, 1, Qt::AlignLeft|Qt::AlignTop);
    _listLayout->setProperty("li-row", m);

    ((CApp*)qApp)->_tjob->_ttype = m-1;

    /*QWidget* topmostWnd = this->parentWidget()->parentWidget()->parentWidget()->parentWidget();
    if( index.row() == 2)
    {
        topmostWnd->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
        topmostWnd->setGeometry(QApplication::desktop()->availableGeometry());
    }
    else
    { QWidget* topmostWnd = this->parentWidget()->parentWidget()->parentWidget()->parentWidget();
        topmostWnd->setFixedSize(960, 720);
        topmostWnd->setGeometry((QApplication::desktop()->availableGeometry().width()- 960)/2,
                                (QApplication::desktop()->availableGeometry().height()- 720)/2,
                                960, 720);
    } */
}


void CAdjustTestWindow::nextList(int i)
{
    if( i == _listLayout->count() - 2 )
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
    QWidget *newwidget = _factory.createObject<QWidget>(_objectMap.key( i+2 ));

    _topLayout->addWidget(newwidget, 0, Qt::AlignLeft|Qt::AlignTop);
    connect(newwidget,SIGNAL(nextList(int)),this,SLOT(nextList(int)));
    connect(newwidget,SIGNAL(newMessage(QString,bool)),this->parent(),SLOT(newMessage(QString,bool)));

    _listLayout->setCurrentRow( i+2 );
    _listLayout->setProperty("li-row",  i+2 );
    ((CApp*)qApp)->_tjob->_ttype = i;
}
