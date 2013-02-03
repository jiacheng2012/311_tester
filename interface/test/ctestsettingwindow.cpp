#include "capp.h"
#include "ctestsettingwindow.h"

CTestSettingWindow::CTestSettingWindow(QWidget *parent) :
    QWidget(parent)
{
    CApp* inst = qobject_cast<CApp*>(qApp);
    topLayout = new QVBoxLayout();
    //***连接设置
    _firstGroup = new QGroupBox(tr("第一步：通讯和配置文件设置"));
    _firstGroup->setMinimumWidth(600);

    QVBoxLayout *fv = new QVBoxLayout(_firstGroup);

    _statusLabel_1 = new QLabel(tr("当前默认设置如下，点击确认进入下一步。"));
    _statusLabel_1->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    _meterFile = new QLineEdit;
    _meterFile->setText( ((CApp*)qApp)->_config->_mfile);
    _meterFile->setReadOnly(true);
    _meterButton = new QPushButton(QIcon(":/res/open.png"), tr("载入配置文件(O)"));
    _meterButton->setStyleSheet("min-width:150px;min-height:25px;");

    _adjustFile = new QLineEdit;
    _adjustFile->setText( ((CApp*)qApp)->_config->_afile);
    _adjustFile->setReadOnly(true);
    _adjustButton = new QPushButton(QIcon(":/res/open.png"), tr("载入调校文件(S)"));
    _adjustButton->setStyleSheet("min-width:150px;min-height:25px;");

    QGridLayout* grid1 = new QGridLayout;
    grid1->addWidget(new QLabel(tr("仪表配置文件：")), 0, 0);
    grid1->addWidget(_meterFile, 0, 1);
    grid1->addWidget(_meterButton, 0, 2);
    grid1->addWidget(new QLabel(tr("仪表调校文件：")), 1, 0);
    grid1->addWidget(_adjustFile, 1, 1);
    grid1->addWidget(_adjustButton, 1, 2);

    QGroupBox *_firstSubGroup = new QGroupBox(tr("信号板通信设置"));
    _plcPortNumber = new QComboBox;//PLC端口号
    for(int i=1;i<10;i++){
        _plcPortNumber->addItem(tr("COM")+QString::number(i));
    }
    _plcPortNumber->setCurrentIndex(inst->_config->_connectParmsMap.value("plc-portnumber"));
    _plcBaudRate = new QComboBox;//PLC波特率
    _plcBaudRate->addItem(tr("9600"));
    _plcBaudRate->addItem(tr("19200"));
    _plcBaudRate->addItem(tr("38400"));
    _plcBaudRate->addItem(tr("57600"));
    _plcBaudRate->addItem(tr("115200"));
    _plcBaudRate->setCurrentIndex(inst->_config->_connectParmsMap.value("plc-baudrate"));
    _plcDataBit = new QComboBox;//PLC数据位
    _plcDataBit->addItem(tr("5"));
    _plcDataBit->addItem(tr("6"));
    _plcDataBit->addItem(tr("7"));
    _plcDataBit->addItem(tr("8"));
    _plcDataBit->setCurrentIndex(inst->_config->_connectParmsMap.value("plc-databit"));
    _plcParityCheck = new QComboBox;//PLC奇偶校验
    _plcParityCheck->addItem(tr("无"));
    _plcParityCheck->addItem(tr("奇校验"));
    _plcParityCheck->addItem(tr("偶校验"));
    _plcParityCheck->setCurrentIndex(inst->_config->_connectParmsMap.value("plc-paritycheck"));
    _plcStopBit = new QComboBox;//PLC停止位
    _plcStopBit->addItem(tr("1"));
    _plcStopBit->addItem(tr("1.5"));
    _plcStopBit->addItem(tr("2"));
    _plcStopBit->setCurrentIndex(inst->_config->_connectParmsMap.value("plc-stopbit"));
    _plcDataFlowControl = new QComboBox;//PLC数据流控制
    _plcDataFlowControl->addItem(tr("无"));
    _plcDataFlowControl->addItem(tr("Xon/Xoff"));
    _plcDataFlowControl->addItem(tr("硬件"));
    _plcDataFlowControl->setCurrentIndex(inst->_config->_connectParmsMap.value("plc-dataflowcontrol"));

    QGridLayout *_firstSubGroupGrid = new QGridLayout(_firstSubGroup);
    _firstSubGroupGrid->addWidget(new QLabel(tr("端口号：")),0,0);
    _firstSubGroupGrid->addWidget(_plcPortNumber,0,1);
    _firstSubGroupGrid->addWidget(new QLabel(tr("波特率：")),1,0);
    _firstSubGroupGrid->addWidget(_plcBaudRate,1,1);
    _firstSubGroupGrid->addWidget(new QLabel(tr("数据位：")),2,0);
    _firstSubGroupGrid->addWidget(_plcDataBit,2,1);
    _firstSubGroupGrid->addWidget(new QLabel(tr("奇偶校验：")),3,0);
    _firstSubGroupGrid->addWidget(_plcParityCheck,3,1);
    _firstSubGroupGrid->addWidget(new QLabel(tr("停止位：")),4,0);
    _firstSubGroupGrid->addWidget(_plcStopBit,4,1);
    _firstSubGroupGrid->addWidget(new QLabel(tr("数据流控制：")),5,0);
    _firstSubGroupGrid->addWidget(_plcDataFlowControl,5,1);

    QGroupBox *_secondSubGroup = new QGroupBox(tr("仪表板通信设置"));
    _meterPortNumber = new QComboBox;//仪表板端口号
    for(int i=1;i<10;i++){
        _meterPortNumber->addItem(tr("COM")+QString::number(i));
    }
    _meterPortNumber->setCurrentIndex(inst->_config->_connectParmsMap.value("meter-portnumber"));
    _meterBaudRate = new QComboBox;//仪表板波特率
    _meterBaudRate->addItem(tr("9600"));
    _meterBaudRate->addItem(tr("19200"));
    _meterBaudRate->addItem(tr("38400"));
    _meterBaudRate->addItem(tr("57600"));
    _meterBaudRate->addItem(tr("115200"));
    _meterBaudRate->setCurrentIndex(inst->_config->_connectParmsMap.value("meter-baudrate"));
    _meterDataBit = new QComboBox;//仪表板数据位
    _meterDataBit->addItem(tr("5"));
    _meterDataBit->addItem(tr("6"));
    _meterDataBit->addItem(tr("7"));
    _meterDataBit->addItem(tr("8"));
    _meterDataBit->setCurrentIndex(inst->_config->_connectParmsMap.value("meter-databit"));
    _meterParityCheck = new QComboBox;//仪表板奇偶校验
    _meterParityCheck->addItem(tr("无"));
    _meterParityCheck->addItem(tr("奇校验"));
    _meterParityCheck->addItem(tr("偶校验"));
    _meterParityCheck->setCurrentIndex(inst->_config->_connectParmsMap.value("meter-paritycheck"));
    _meterStopBit = new QComboBox;//仪表板停止位
    _meterStopBit->addItem(tr("1"));
    _meterStopBit->addItem(tr("1.5"));
    _meterStopBit->addItem(tr("2"));
    _meterStopBit->setCurrentIndex(inst->_config->_connectParmsMap.value("meter-stopbit"));
    _meterDataFlowControl = new QComboBox;//仪表板数据流控制
    _meterDataFlowControl->addItem(tr("无"));
    _meterDataFlowControl->addItem(tr("Xon/Xoff"));
    _meterDataFlowControl->addItem(tr("硬件"));
    _meterDataFlowControl->setCurrentIndex(inst->_config->_connectParmsMap.value("meter-dataflowcontrol"));

    QGridLayout *_secondSubGroupGrid = new QGridLayout(_secondSubGroup);
    _secondSubGroupGrid->addWidget(new QLabel(tr("端口号：")),0,0);
    _secondSubGroupGrid->addWidget(_meterPortNumber,0,1);
    _secondSubGroupGrid->addWidget(new QLabel(tr("波特率：")),1,0);
    _secondSubGroupGrid->addWidget(_meterBaudRate,1,1);
    _secondSubGroupGrid->addWidget(new QLabel(tr("数据位：")),2,0);
    _secondSubGroupGrid->addWidget(_meterDataBit,2,1);
    _secondSubGroupGrid->addWidget(new QLabel(tr("奇偶校验：")),3,0);
    _secondSubGroupGrid->addWidget(_meterParityCheck,3,1);
    _secondSubGroupGrid->addWidget(new QLabel(tr("停止位：")),4,0);
    _secondSubGroupGrid->addWidget(_meterStopBit,4,1);
    _secondSubGroupGrid->addWidget(new QLabel(tr("数据流控制：")),5,0);
    _secondSubGroupGrid->addWidget(_meterDataFlowControl,5,1);

    QHBoxLayout *fvHLayout = new QHBoxLayout;
    fvHLayout->addWidget(_firstSubGroup);
    fvHLayout->addWidget(_secondSubGroup);
    fvHLayout->setSpacing(60);
    fvHLayout->setContentsMargins(30,15,30,15);
    _okSetButton = new QPushButton(tr("确认设置并进入下一步"));
    _okSetButton->setStyleSheet("font:15px;min-width:180px;min-height:26px;");

    fv->addLayout(grid1);
    fv->addLayout(fvHLayout);
    fv->addWidget(_okSetButton, 0, Qt::AlignCenter);

    //***扫描序列号
    _idText = new QLineEdit();
    _idButton = new QPushButton(tr("检索检测记录"));
    _idButton->setStyleSheet("font:15px;min-width:160px;min-height:26px;");
    _scanButton = new QPushButton(tr("...扫描序列号"));
    _scanButton->setStyleSheet("font:15px;min-width:140px;min-height:26px;");
    _secondGroup = new QGroupBox(tr("第二步：仪表检测记录检索"));
    _secondGroup->setFixedSize(400,120);
    _secondGroup->setVisible(false);
    QGridLayout* l1 = new QGridLayout(_secondGroup);

    l1->addWidget(new QLabel(tr("仪表序列号：")),0,0);
    l1->addWidget(_idText,0,1);
    l1->addWidget(_scanButton,0,2);
    l1->addWidget(_idButton,1,1);

    //***检测记录检索
    _resultText = new QLabel();
    //_testList = new QTableWidget(1,8);
    _thirdGroup = new QGroupBox(tr("第三步：选择检测"));

    _testList = new QTableWidget;
    _testList->setColumnCount(8);
    QStringList header;
    header<<tr("检测号")<<tr("日期")<<tr("检测员")<<tr("初检")<<tr("全检")<<tr("调校")<<tr("功能全检")<<tr("备注");
    _testList->horizontalHeader()->setMovable(false);
    _testList->horizontalHeader()->setClickable(false);
    _testList->setHorizontalHeaderLabels(header);
    _testList->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    _testList->horizontalHeader()->setResizeMode(7,QHeaderView::Stretch);
    _testList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _testList->setSelectionBehavior(QAbstractItemView::SelectRows);
    _testList->setSelectionMode(QAbstractItemView::SingleSelection);
    _testList->setFocusPolicy(Qt::NoFocus);
    _testList->setFixedSize(600,120);

    QVBoxLayout* l2 = new QVBoxLayout(_thirdGroup);
    l2->addWidget(_resultText);
    l2->addWidget(_testList);

    _thirdGroup->setMinimumSize(600,200);
    _thirdGroup->setVisible(false);

    topLayout->addWidget(_firstGroup, 0, Qt::AlignTop|Qt::AlignHCenter);
    topLayout->addWidget(_secondGroup, 0, Qt::AlignTop|Qt::AlignHCenter);
    topLayout->addWidget(_thirdGroup, 0, Qt::AlignTop|Qt::AlignHCenter);
    setLayout(topLayout);

    //***Signal
    connect(_meterButton,SIGNAL(clicked()),this,SLOT(meterButtonClicked()));
    connect(_adjustButton,SIGNAL(clicked()),this,SLOT(adjustButtonClicked()));
    connect(_idButton,SIGNAL(clicked()),this,SLOT(idButtonClicked()));
    connect((CApp*)qApp,SIGNAL(updateTestJobSetting()),this,SLOT(updateTestJobSetting()));
    connect(_testList,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(listDoubleClicked(int,int)));
    connect(_okSetButton,SIGNAL(clicked()),this,SLOT(okSetButtonClicked()));
    connect(this,SIGNAL(toTestTabPage(int)),(CApp*)qApp,SIGNAL(toTestTabPage(int)));
    connect((CApp*)qApp,SIGNAL(updateTestJobSetting()),this,SLOT(updateTestJobSetting()));
}

void CTestSettingWindow::idButtonClicked()
{
    if(_idText->text().length()>0 &&_idText->text().length()<10 )
        ((CApp*)qApp)->addTestJobFromID(_idText->text());
    ((CApp*)qApp)->sleep(5); //适当延时，让scrollarea自动变长后再自动滚动
    QScrollArea* pscroll = qobject_cast<QScrollArea*>(parentWidget()->parentWidget());
    pscroll->verticalScrollBar()->setValue(300);

    ((CApp*)qApp)->_meter = new CMeter("014758");
    int iRet = QMessageBox::question(this, "警告", "选择了检测批号 014758，点击确定开始检测，点击否认重新选择。",
                                     QMessageBox::Yes, QMessageBox::No);
    if(iRet == QMessageBox::No)
        return;
    else
    {
        ((CApp*)qApp)->startJob();
        ((CApp*)qApp)->_tjob->_mconfig = _mconfig;
        ((CApp*)qApp)->_tjob->_aconfig = _aconfig;
 //       ((CApp*)qApp)->_tjob->_tr = t[r];

 //       setProperty("testid", _testList->item(r,0)->text());
        emit toTestTabPage(FULLTEST_ID);
    }
}

void CTestSettingWindow::meterButtonClicked()
{
    QFileDialog dlg(this,"打开仪表配置文件",_meterFile->text(), tr("配置文件 (*.ini)"));
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);

    if(dlg.exec() == QFileDialog::FileName)
    {
        setProperty("mfstring",dlg.selectedFiles().first());
        _meterFile->setText( dlg.selectedFiles().first() );
    }
}

void CTestSettingWindow::adjustButtonClicked()
{
    QFileDialog dlg(this,"打开仪表调校配置文件",_adjustFile->text(), tr("配置文件 (*.ini)"));
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);

    if(dlg.exec() == QFileDialog::FileName)
    {
        setProperty("afstring",dlg.selectedFiles().first());
        _adjustFile->setText( dlg.selectedFiles().first() );
    }
}

void CTestSettingWindow::okSetButtonClicked()
{
    bool err=false;
    _mconfig = new CMeterConfig();
    if( _mconfig->readFile( _meterFile->text() ) )
    {
        //      QMessageBox::information(this,"",tr("仪表配置文件 ")+ filename + tr(" 读取成功！"));
        QString iniFilePath = ((CApp*)qApp)->applicationDirPath()+QString("/main.ini");
        QSettings *sIniSetting = new QSettings(iniFilePath, QSettings::IniFormat);
        sIniSetting->setIniCodec("UTF-8");
        sIniSetting->setValue("config_file/meterfile",_meterFile->text());
        sIniSetting->sync();
        delete sIniSetting;
        ((CApp*)qApp)->_config->readFile(iniFilePath);
    }
    else
    {
        err = true;
        _statusLabel_1->setText(tr("仪表配置文件读取错误，请重新选择正确的仪表配置文件！"));
        QMessageBox::information(this,"",tr("仪表配置文件读取错误！"));
    }


    _aconfig = new CAdjustConfig();
    if( _aconfig->readFile( _adjustFile->text()) )
    {
        //      QMessageBox::information(this,"",tr("仪表调校文件 ")+ filename + tr(" 读取成功！"));
        QString iniFilePath = ((CApp*)qApp)->applicationDirPath()+QString("/main.ini");
        QSettings *sIniSetting = new QSettings(iniFilePath, QSettings::IniFormat);
        sIniSetting->setIniCodec("UTF-8");
        sIniSetting->setValue("config_file/adjustfile",_adjustFile->text());
        sIniSetting->sync();
        delete sIniSetting;
        ((CApp*)qApp)->_config->readFile(iniFilePath);
    }
    else
    {
        err = true;
        _statusLabel_1->setText(tr("仪表调校文件读取错误，请重新选择正确的仪表配置文件！"));
        QMessageBox::information(this,"",tr("仪表调校文件读取错误！"));
    }

    //***通讯设置检查
    QString iniFilePath = ((CApp*)qApp)->applicationDirPath()+QString("/main.ini");
    QSettings *sIniSetting = new QSettings(iniFilePath, QSettings::IniFormat);
    sIniSetting->setIniCodec("UTF-8");
    sIniSetting->setValue("plcset/portnumber",_plcPortNumber->currentIndex());
    sIniSetting->setValue("plcset/baudrate",_plcBaudRate->currentIndex());
    sIniSetting->setValue("plcset/databit",_plcDataBit->currentIndex());
    sIniSetting->setValue("plcset/paritycheck",_plcParityCheck->currentIndex());
    sIniSetting->setValue("plcset/stopbit",_plcStopBit->currentIndex());
    sIniSetting->setValue("plcset/dataflowcontrol",_plcDataFlowControl->currentIndex());

    sIniSetting->setValue("meterset/portnumber",_meterPortNumber->currentIndex());
    sIniSetting->setValue("meterset/baudrate",_meterBaudRate->currentIndex());
    sIniSetting->setValue("meterset/databit",_meterDataBit->currentIndex());
    sIniSetting->setValue("meterset/paritycheck",_meterParityCheck->currentIndex());
    sIniSetting->setValue("meterset/stopbit",_meterStopBit->currentIndex());
    sIniSetting->setValue("meterset/dataflowcontrol",_meterDataFlowControl->currentIndex());

    ((CApp*)qApp)->_config->readFile(iniFilePath);

    if(!err)
        _secondGroup->setVisible(true);
}

void CTestSettingWindow::updateTestJobSetting()
{
    QVector<testRecord> t = ((CApp*)qApp)->_meter->_tr;
    _resultText->setText(tr("仪表的检测记录如下：(点亮项为本次测试，其它为过往测试，双击选择一条记录开始测试！)"));
    _resultText->setStyleSheet("font:bold 14px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;padding:0 15px;");
    _thirdGroup->setVisible(true);
    QString r1,r2,r3,r4;
    _testList->clearContents();
    _testList->setRowCount(t.size());

    for(int i = 0; i != t.size() ; ++i)
    {
        r1 = QString::number(getTestSucessPercentage(INIT_TEST_COUNT,t[i]._inittest)) + "%";
        r2 = QString::number(getTestSucessPercentage(FULL_TEST_COUNT,t[i]._fulltest)) + "%";
        r3 = QString::number(getTestSucessPercentage(ADJUST_TEST_COUNT,t[i]._adjusttest)) + "%";
        r4 = QString::number(getTestSucessPercentage(FUNCTION_TEST_COUNT,t[i]._functiontest)) + "%";
        _testList->setItem(i,0,new QTableWidgetItem(QString::number(t[i]._id)));
        _testList->setItem(i,1,new QTableWidgetItem(QDateTime::fromTime_t(t[i]._date).toString("yyyy-MM-dd hh:mm:ss")));
        _testList->setItem(i,2,new QTableWidgetItem(t[i]._user));
        _testList->setItem(i,3,new QTableWidgetItem(r1));
        _testList->setItem(i,4,new QTableWidgetItem(r2));
        _testList->setItem(i,5,new QTableWidgetItem(r3));
        _testList->setItem(i,6,new QTableWidgetItem(r4));

        QTableWidgetItem* comment = new QTableWidgetItem(t[i]._comment);
        comment->setToolTip(t[i]._comment);
        _testList->setItem(i,7,comment);
    }
    _testList->sortItems(1, Qt::DescendingOrder);
    _testList->selectRow(0);
    _testList->resizeColumnsToContents();
    _testList->resizeRowsToContents();
}

void CTestSettingWindow::listDoubleClicked(int r,int c)
{
    QVector<testRecord> t = ((CApp*)qApp)->_meter->_tr;
    int iRet = QMessageBox::question(this, "警告", "选择了检测批号 "+_testList->item(r,0)->text()+" ，点击确定开始检测，点击否认重新选择。",
                                     QMessageBox::Yes, QMessageBox::No);
    if(iRet == QMessageBox::No)
        return;
    else
    {
        ((CApp*)qApp)->startJob();
        ((CApp*)qApp)->_tjob->_mconfig = _mconfig;
        ((CApp*)qApp)->_tjob->_aconfig = _aconfig;
        ((CApp*)qApp)->_tjob->_tr = t[r];

        setProperty("testid", _testList->item(r,0)->text());
        emit toTestTabPage(INITTEST_ID);
    }
}

void CTestSettingWindow::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CTestSettingWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {

        }
        else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_O)
        {
            meterButtonClicked();
        }
        else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S)
        {
            adjustButtonClicked();
        }
    }
}
