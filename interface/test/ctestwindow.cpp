#include "capp.h"
#include "ctestwindow.h"
#include "database/cdatabase.h"
#include "database/cuser.h"

CTestWindow::CTestWindow(QWidget *parent)
    : QWidget(parent)
{
    //***
    //ww = QFontMetrics(this->font()).width('w');
    //***UI Component
    topLayout = new QVBoxLayout;
    upLayout = new QHBoxLayout;
    topLayout->setContentsMargins(10,5,10,10);
    QGridLayout *upLeftGrid = new QGridLayout;
    //***整个测试窗口 顶部信息（包含时间、测试人、仪表号、连接状态）
    _logText = new QTextEdit();
    _logText->setReadOnly(true);
    _logText->setFixedHeight(75);
    _logText->setContextMenuPolicy(Qt::CustomContextMenu);

    _testTimer = new QLabel;
    _testorName = new QLabel;
    _testedMeterNO = new QLabel;
    upLeftGrid->addWidget(new QLabel(tr("时间：")),0,0);
    upLeftGrid->addWidget(new QLabel(tr("测试员：")),1,0);
    upLeftGrid->addWidget(new QLabel(tr("仪表：")),2,0);
    upLeftGrid->addWidget(_testTimer,0,1);
    upLeftGrid->addWidget(_testorName,1,1);
    upLeftGrid->addWidget(_testedMeterNO,2,1);

    _plcConnectStatus = new QLabel(tr("断"));
    _plcConnectStatus->setStyleSheet("border-radius:25px;background:red;color:wheat;border:1px solid #666;");
    _plcConnectStatus->setAlignment(Qt::AlignCenter);
    _plcConnectStatus->setFixedSize(50,50);
    _meterConnectStatus = new QLabel(tr("断"));
    _meterConnectStatus->setStyleSheet("border-radius:25px;background:red;color:wheat;border:1px solid #666;");
    _meterConnectStatus->setAlignment(Qt::AlignCenter);
    _meterConnectStatus->setFixedSize(50,50);

    _dbConnectStatus = new QLabel(tr("断"));
    _dbConnectStatus->setAlignment(Qt::AlignCenter);
    _dbConnectStatus->setStyleSheet("border-radius:25px;background:red;color:wheat;border:1px solid #666;");
    _dbConnectStatus->setFixedSize(50,50);
    QGridLayout *upRightGrid = new QGridLayout;
    upRightGrid->addWidget(new QLabel(tr("数据库连接状态")),1,0);
    upRightGrid->addWidget(new QLabel(tr("PLC连接状态")),1,1);
    upRightGrid->addWidget(new QLabel(tr("仪表板连接状态")),1,2);
    upRightGrid->addWidget(_dbConnectStatus,0,0,Qt::AlignCenter);
    upRightGrid->addWidget(_plcConnectStatus,0,1,Qt::AlignCenter);
    upRightGrid->addWidget(_meterConnectStatus,0,2,Qt::AlignCenter);

    upLayout->addWidget(_logText,1);
    upLayout->addItem(new QSpacerItem(35,20));
    upLayout->addLayout(upLeftGrid);
    upLayout->addLayout(upRightGrid);
    upLayout->setContentsMargins(10,0,10,5);
    upLayout->setSpacing(5);

    _factory.registerObject<CTestSettingWindow>("CTestSettingWindow");
    _factory.registerObject<CBootloaderWindow>("CBootloaderWindow");
    _factory.registerObject<CInitTestWindow>("CInitTestWindow");
    _factory.registerObject<CFullTestWindow>("CFullTestWindow");
    _factory.registerObject<CAdjustTestWindow>("CAdjustTestWindow");
    _factory.registerObject<CFunctionTestWindow>("CFunctionTestWindow");
    _factory.registerObject<CTestSummaryWindow>("CTestSummaryWindow");

    _objectMap.clear();
    QStringList classNames;
    classNames << "CTestSettingWindow" << "CBootloaderWindow"<< "CInitTestWindow"
               << "CFullTestWindow"<< "CAdjustTestWindow"<< "CFunctionTestWindow";
    foreach(QString name, classNames)
    {
        _objectMap[name] = classNames.indexOf(name);
    }
    //***TabBar Component
    groupBar = new QTabBar();
    groupBar->installEventFilter(this);

    QScrollArea* _scrollView = new QScrollArea;
    _scrollView ->setWidget( new CTestSettingWindow );
    _scrollView ->setWidgetResizable(true);
    _scrollView ->setAlignment( Qt::AlignCenter);

    groupBar->addTab(tr("步骤一：作业设置"));
    groupBar->addTab(tr("步骤二：仪表单板初检"));
    groupBar->addTab(tr("步骤三：Bootloader程序下载"));
    groupBar->addTab(tr("步骤四：仪表硬件全检"));
    groupBar->addTab(tr("步骤五：仪表调校"));
    groupBar->addTab(tr("步骤六：仪表功能全检"));

    groupBar->setCurrentIndex(0);
    _currentTabIndex = 0;
    topLayout->addLayout(upLayout);
    topLayout->addWidget(groupBar, 0, 0);
    topLayout->addWidget(_scrollView,1,0);
    setLayout(topLayout);

    //***Signal
    connect(_logText,SIGNAL(customContextMenuRequested(const QPoint&)), this,SLOT(showContextMenu(const QPoint&)));
    connect((CApp*)qApp,SIGNAL(newMessage(QString,bool)),this,SLOT(newMessage(QString,bool)));
    connect((CApp*)qApp,SIGNAL(toTestTabPage(int)),this,SLOT(toTestTabPage(int)));
    //connect(groupBar,SIGNAL(currentChanged(int)),this,SLOT(currentChanged(int)));
    connect((CApp*)qApp,SIGNAL(_232SetupFullTest()),this,SLOT(_232Setup()));
    connect((CApp*)qApp,SIGNAL(_232SetupAdjustTest()),this,SLOT(_232Setup()));
    connect((CApp*)qApp,SIGNAL(_232SetupFunctionTest()),this,SLOT(_232Setup()));
    connect((CApp*)qApp,SIGNAL(_485SetupFullTest()),this,SLOT(_485Setup()));
    connect((CApp*)qApp,SIGNAL(_485SetupAdjustTest()),this,SLOT(_485Setup()));
    connect((CApp*)qApp,SIGNAL(_485SetupFunctionTest()),this,SLOT(_485Setup()));
    connect((CApp*)qApp,SIGNAL(databaseSetup()),this,SLOT(databaseSetup()));
    connect((CApp*)qApp,SIGNAL(stop232()),this,SLOT(stop232()));
    connect((CApp*)qApp,SIGNAL(stop485()),this,SLOT(stop485()));
    connect((CApp*)qApp,SIGNAL(jobSetup(QString,QString,QString)),this,SLOT(jobSetup(QString,QString,QString)));

    if(((CApp*)qApp)->_db)
        databaseSetup();

    //timerEvent(0);
    startTimer(1000);
}

CTestWindow::~CTestWindow()
{
}

void CTestWindow::timerEvent(QTimerEvent* event)
{
    QDateTime dt=QDateTime::currentDateTime();
    QDate date=dt.date();
    QTime time=dt.time();
    QString dtstr=QString().sprintf("%04d-%02d-%02d %02d:%02d:%02d",date.year(),date.month(),date.day(),time.hour(),time.minute(),time.second());
    _testTimer->setText(dtstr);
}

void CTestWindow::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}

void CTestWindow::showContextMenu(const QPoint& pos)
{
    QMenu contextMenu(tr("text edit contextmenu"), _logText);
    QAction* action = new QAction(tr("清空内容"), _logText);
    contextMenu.addAction(action);
    connect(action, SIGNAL(triggered()), _logText, SLOT(clear()));
    contextMenu.exec(mapToGlobal(pos));
    delete action;
}

void CTestWindow::newMessage(QString msg, bool showOnScreen)
{
    const int maxBufferSize = 10000;

    if (showOnScreen)
        QMessageBox::information(this,tr(""),msg);

    QString txt = _logText->toPlainText() + "\n" +
            QDateTime::currentDateTime().toUTC().toString("[yy-MM-dd hh:mm:ss] ") + msg;
    _logText->clear();
    _logText->append(txt.right(maxBufferSize));
}

void CTestWindow::toTestTabPage(int page)
{
    //currentTabIndex = page;
    QLayoutItem* item = 0;
    QWidget* widget = 0;
    if( (item = topLayout->itemAt(2)) && (widget = topLayout->itemAt(2)->widget()) )
    {
        widget->hide();
        delete widget;
    }
    groupBar->setCurrentIndex(page);
    QWidget *newwidget = _factory.createObject<QWidget>(_objectMap.key(page));
    topLayout->addWidget(newwidget, 2, Qt::AlignLeft|Qt::AlignTop);
    connect(newwidget,SIGNAL(newMessage(QString,bool)),this,SLOT(newMessage(QString,bool)));
}

void CTestWindow::_232Setup()
{
    _meterConnectStatus->setText(tr("通"));
    _meterConnectStatus->setStyleSheet("border-radius:25px;background:green;color:wheat;border:1px solid #666;");
}

void CTestWindow::stop232()
{
    _meterConnectStatus->setText(tr("断"));
    _meterConnectStatus->setStyleSheet("border-radius:25px;background:red;color:wheat;border:1px solid #666;");
}

void CTestWindow::_485Setup()
{
    _plcConnectStatus->setText(tr("通"));
    _plcConnectStatus->setStyleSheet("border-radius:25px;background:green;color:wheat;border:1px solid #666;");
}

void CTestWindow::stop485()
{
    _plcConnectStatus->setText(tr("断"));
    _plcConnectStatus->setStyleSheet("border-radius:25px;background:red;color:wheat;border:1px solid #666;");
}

void CTestWindow::databaseSetup()
{
    _dbConnectStatus->setText(tr("通"));
    _dbConnectStatus->setStyleSheet("border-radius:25px;background:green;color:wheat;border:1px solid #666;");
}

void CTestWindow::jobSetup(QString meter,QString id,QString name)
{
    _testorName->setText(name + "(" + id + ")");
    _testedMeterNO->setText(meter);
}

void CTestWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {
            QList<QPushButton *> pushButtons = findChildren<QPushButton *>();
            foreach(QPushButton* pb,pushButtons){
                if(pb->text().compare(tr("保存结果&进入下一项检测")) == 0 && pb->isVisible())
                {
                    QWidget *widget = pb->parentWidget()->parentWidget();
                    widget->metaObject()->invokeMethod(widget,"nextButtonClicked");
                }
            }
        }
    }
}

bool CTestWindow::eventFilter(QObject *obj, QEvent *event)
{
    if( event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if( mouseEvent->buttons() & Qt::LeftButton )
        {
            QTabBar* tab = qobject_cast<QTabBar*>(obj);
            int tabIndex = 0;
            for(int i=0; i < tab->count(); ++i)
            {
                QRect rect = tab->tabRect(i);
                if( rect.contains( mouseEvent->pos() ) )
                {
                    tabIndex = i;
                    break;
                }
            }
            if(tabIndex == groupBar->currentIndex())
            {
                event->ignore();
                return true;
            }
            if(tabIndex != 0)
            {
                if( ((CApp*)qApp)->_tjob == 0 )
                {
                    newMessage(tr("请先设置检测项目"),1);
                    //_currentTabIndex = 0;
                    event->ignore();
                    return true;
                }

                if( ((CApp*)qApp)->_tjob->_jtype != NONE_ID )
                {
                    ((CApp*)qApp)->_tjob->clearJob();
                }

                this->parentWidget()->setFixedSize(900, 720);
                this->parentWidget()->setGeometry((QApplication::desktop()->availableGeometry().width()- 900)/2,
                                                  (QApplication::desktop()->availableGeometry().height()- 720)/2,
                                                  900, 720);
                {
                    QLayoutItem* item = 0;
                    QWidget* widget = 0;
                    if( (item = topLayout->itemAt(2)) && (widget = topLayout->itemAt(2)->widget()) )
                    {
                        widget->hide();
                        delete widget;
                    }
                }
                //***新创建 widget
                QWidget *newwidget = _factory.createObject<QWidget>(_objectMap.key(tabIndex));
                if(tabIndex != 6)
                    topLayout->addWidget(newwidget, 2, Qt::AlignLeft|Qt::AlignTop);
                else
                {
                    topLayout->addWidget(newwidget, 2, Qt::AlignHCenter);
                    //_jsyWnd->updateTestSummary();
                }
                connect(newwidget,SIGNAL(newMessage(QString,bool)),this,SLOT(newMessage(QString,bool)));
            }
            else
            {
                CTestJob *tj = ((CApp*)qApp)->_tjob;

                if(tj!=0)
                {
                    int iRet = QMessageBox::question(this, "警告", "检测进行中！确定开始另外一款仪表的检测吗？",
                                                     QMessageBox::Yes, QMessageBox::No);
                    if(iRet == QMessageBox::No)
                    {
                        event->ignore();
                        return true;
                    }

                    //***
                    ((CApp*)qApp)->deleteJob();
                }

                QLayoutItem* item = 0;
                QWidget* widget = 0;
                if( (item = topLayout->itemAt(2)) && (widget = topLayout->itemAt(2)->widget()) )
                {
                    widget->hide();
                    delete widget;
                }

                QScrollArea* _scrollView = new QScrollArea;
                _scrollView ->setWidget( new CTestSettingWindow );
                _scrollView ->setWidgetResizable(true);
                _scrollView ->setAlignment( Qt::AlignCenter);

                topLayout->addWidget(_scrollView,1,0);
            }
        }
    }
    return QWidget::eventFilter( obj, event );
}
