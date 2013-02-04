#include "capp.h"
#include "cfulltestwindow.h"
#include <QTimer>

CFullTestWindow::CFullTestWindow(QWidget *parent) :
    QWidget(parent)
{
    //***UI Component

    _listLayout = new QListWidget;
    _listLayout->addItem(tr("测试设置"));
    _listLayout->addItem(tr("指示灯测试"));
    _listLayout->addItem(tr("液晶屏背光测试"));
    _listLayout->addItem(tr("液晶屏测试"));
    _listLayout->addItem(tr("按键测试"));
    _listLayout->addItem(tr("存储测试"));
    _listLayout->addItem(tr("自供电测试"));
    _listLayout->addItem(tr("仪表电源电压采样测试"));
    _listLayout->addItem(tr("步进电机测试"));
    _listLayout->addItem(tr("CAN测试"));
    _listLayout->addItem(tr("32路IO输入测试"));
    _listLayout->addItem(tr("8路IO输出测试"));
    _listLayout->addItem(tr("模拟量测试"));
    _listLayout->addItem(tr("频率量测试"));
    _listLayout->setFixedWidth(140);
    _listLayout->setFixedHeight(300);
    _listLayout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    _listLayout->setCurrentRow(0);
    _listLayout->setProperty("li-row", 0);

    _factory.registerObject<CFullTestOverviewWindow>("CFullTestOverviewWindow");
    _factory.registerObject<CFullTestLEDWindow>("CFullTestLEDWindow");
    _factory.registerObject<CFullTestLCDBackLightWindow>("CFullTestLCDBackLightWindow");
    _factory.registerObject<CFullTestLCDWindow>("CFullTestLCDWindow");
    _factory.registerObject<CFullTestKeyWindow>("CFullTestKeyWindow");
    _factory.registerObject<CFullTestMemoryWindow>("CFullTestMemoryWindow");
    _factory.registerObject<CFullTestSelfPowerWindow>("CFullTestSelfPowerWindow");
    _factory.registerObject<CFullTestMeterVoltageWindow>("CFullTestMeterVoltageWindow");
    _factory.registerObject<CFullTestMotorWindow>("CFullTestMotorWindow");
    _factory.registerObject<CFullTestCanWindow>("CFullTestCanWindow");
    _factory.registerObject<CFullTestIOInWindow>("CFullTestIOInWindow");
    _factory.registerObject<CFullTestIOOutWindow>("CFullTestIOOutWindow");
    _factory.registerObject<CFullTestAnalogWindow>("CFullTestAnalogWindow");
    _factory.registerObject<CFullTestFreqWindow>("CFullTestFreqWindow");

    _objectMap.clear();
    QStringList classNames;
    classNames << "CFullTestOverviewWindow" << "CFullTestLEDWindow"<< "CFullTestLCDBackLightWindow"
               << "CFullTestLCDWindow"<< "CFullTestKeyWindow"<< "CFullTestMemoryWindow"<< "CFullTestSelfPowerWindow"
               << "CFullTestMeterVoltageWindow"<< "CFullTestMotorWindow"<< "CFullTestCanWindow"<< "CFullTestIOInWindow"
               << "CFullTestIOOutWindow"<< "CFullTestAnalogWindow" << "CFullTestFreqWindow";
    foreach(QString name, classNames)
    {
        _objectMap[name] = classNames.indexOf(name);
    }

    _topLayout = new QHBoxLayout;

    _topLayout->addWidget(_listLayout, 0, Qt::AlignLeft|Qt::AlignTop);
    _topLayout->addWidget(new CFullTestOverviewWindow, 0, Qt::AlignLeft|Qt::AlignTop);
    //_topLayout->setSizeConstraint(QLayout::SetFixedSize);
//    _topLayout->setSizeConstraint(QLayout::SetMaximumSize);
    _topLayout->setContentsMargins(15, 10, 15, 10);
    setLayout(_topLayout);

    //***Signal
    connect(_listLayout, SIGNAL(clicked(QModelIndex)),this, SLOT(listLayoutClicked(QModelIndex)));
}

void CFullTestWindow::listLayoutClicked(QModelIndex index)
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

    //***发送空帧
    hardwareTestFrame_0x30 b={_METER_FRAME_HARDWARE_TEST_,0x00};
    ((CApp*)qApp)->_tjob->setupSendFrame232(data30FromFrame(b),_MS_50_);
    //***clear 485
    PLCCommandFrame_0x80 c={0};
    ((CApp*)qApp)->_tjob->initFrame80(&c);
    QByteArray tmp=QByteArray::fromRawData((const char*)&c,sizeof(c));
    ((CApp*)qApp)->_tjob->setupSendFrame485(tmp,_MS_50_);
    setProperty("row",index.row());
    QTimer::singleShot(_MS_300_, this, SLOT(delayClick()));
}

void CFullTestWindow::delayClick()
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
 //   qDebug() << m << parentWidget()->metaObject()->className();
 //    qDebug() << parentWidget()->parentWidget()->metaObject()->className();
    if( m != 1)
    {
        this->parentWidget()->parentWidget()->setFixedSize(900, 720);
        this->parentWidget()->parentWidget()->setGeometry((QApplication::desktop()->availableGeometry().width()- 900)/2,
                                          (QApplication::desktop()->availableGeometry().height()- 720)/2,
                                          900, 720);
    }
    else
    {
        this->parentWidget()->parentWidget()->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
        this->parentWidget()->parentWidget()->setGeometry(QApplication::desktop()->availableGeometry());
    }
    _topLayout->addWidget(newwidget, 0, Qt::AlignLeft|Qt::AlignTop);
    _listLayout->setProperty("li-row", m);
    connect(newwidget,SIGNAL(nextList(int)),this,SLOT(nextList(int)));
    connect(newwidget,SIGNAL(newMessage(QString,bool)),this->parent(),SLOT(newMessage(QString,bool)));

    ((CApp*)qApp)->_tjob->_ttype = m-1;
}

void CFullTestWindow::nextList(int i)
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

    _topLayout->addWidget(newwidget, 1, Qt::AlignLeft|Qt::AlignAbsolute);
    connect(newwidget,SIGNAL(nextList(int)),this,SLOT(nextList(int)));
    connect(newwidget,SIGNAL(newMessage(QString,bool)),this->parent(),SLOT(newMessage(QString,bool)));

    _listLayout->setCurrentRow( i+2 );
    _listLayout->setProperty("li-row",  i+2 );
    ((CApp*)qApp)->_tjob->_ttype = i;
}
