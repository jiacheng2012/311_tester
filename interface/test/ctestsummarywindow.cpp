#include "capp.h"
#include "ctestsummarywindow.h"

CTestSummaryWindow::CTestSummaryWindow(QWidget *parent) :
    QWidget(parent)
{   
    QVBoxLayout* topLevelLayout = new QVBoxLayout(this);

    QGroupBox* main = new QGroupBox(tr("作业总结"));
    main->setFixedSize(700,520);

    QVBoxLayout* mainLayout = new QVBoxLayout(main);
    mainLayout->setContentsMargins(40,15,40,15);
    mainLayout->setSpacing(20);

    _testSummaryTree = new QTreeWidget;
    _testSummaryTree->setColumnCount(2);
    _testSummaryTree->setHeaderLabels(QStringList() << tr("检测项目") << tr("完成情况") << tr("备注"));
    _testSummaryTree->header()->setStretchLastSection( true);
    _testSummaryTree->header()->setResizeMode( QHeaderView::ResizeToContents );
    _testSummaryTree->model()->setHeaderData(2, Qt::Horizontal, Qt::AlignHCenter, Qt::TextAlignmentRole);
    _testSummaryTree->setSelectionMode(QAbstractItemView::NoSelection);
    _testSummaryTree->setFocusPolicy(Qt::NoFocus);

    QLabel* text = new QLabel(tr("本次测试结果统计表"));
    text->setStyleSheet("font:bold 14px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;padding:0 15px;");

    QPushButton* button = new QPushButton(tr("保存本次记录"));
    button->setStyleSheet("font:14px;min-width:100px;min-height:26px;");

    mainLayout->addWidget(text, 0, Qt::AlignCenter);
    mainLayout->addWidget(_testSummaryTree);
    mainLayout->addWidget(button, 0, Qt::AlignCenter);

    topLevelLayout->addWidget(main, 0, Qt::AlignCenter);
    topLevelLayout->setAlignment(Qt::AlignCenter);
    //***signal
    updateTestSummary();
    connect(button, SIGNAL(clicked()), this, SLOT(finishTestJob()));
}

void CTestSummaryWindow::updateTestSummary()
{
    _testSummaryTree->clear();
    int i;

    QCheckBox* check;

    for(i=0;i <= INIT_TEST_COUNT;i++)
    {
        if(i == 0)
            _initTreeItem[i] = new QTreeWidgetItem(_testSummaryTree,QStringList(QString(tr(initTArray[i]._title.toLatin1().data()))));
        else
        {
            _initTreeItem[i] = new QTreeWidgetItem(_initTreeItem[0],QStringList(QString(tr(initTArray[i]._title.toLatin1().data()))));
            //_testSummaryTree->model()->setData(QModelIndex(1), Qt::TextAlignmentRole, Qt::AlignCenter);

            _testSummaryTree->model()->setData(_testSummaryTree->model()->index(0,i), Qt::TextAlignmentRole, Qt::AlignCenter);
        }
        check = new QCheckBox;
        check->setEnabled(false);
        _testSummaryTree->setItemWidget(_initTreeItem[i], 1, check);
    }

    for(i=0; i <= FULL_TEST_COUNT;i++)
    {
        if(i == 0)
            _fullTreeItem[i] = new QTreeWidgetItem(_testSummaryTree,QStringList(QString(tr(fullTArray[i]._title.toLatin1().data()))));
        else
            _fullTreeItem[i] = new QTreeWidgetItem(_fullTreeItem[0],QStringList(QString(tr(fullTArray[i]._title.toLatin1().data()))));
        check = new QCheckBox;
        check->setEnabled(false);
        _testSummaryTree->setItemWidget(_fullTreeItem[i], 1, check);
    }

    for(i=0; i <= ADJUST_TEST_COUNT;i++)
    {
        if(i == 0)
            _adjustTreeItem[i] = new QTreeWidgetItem(_testSummaryTree,QStringList(QString(tr(adjustTArray[i]._title.toLatin1().data()))));
        else
            _adjustTreeItem[i] = new QTreeWidgetItem(_adjustTreeItem[0],QStringList(QString(tr(adjustTArray[i]._title.toLatin1().data()))));
        check = new QCheckBox;
        check->setEnabled(false);
        _testSummaryTree->setItemWidget(_adjustTreeItem[i], 1, check);
    }

    for(i=0;i <= FUNCTION_TEST_COUNT;i++)
    {
        if(i == 0)
            _functionTreeItem[i] = new QTreeWidgetItem(_testSummaryTree,QStringList(QString(tr(functionTArray[i]._title.toLatin1().data()))));
        else
            _functionTreeItem[i] = new QTreeWidgetItem(_functionTreeItem[0],QStringList(QString(tr(functionTArray[i]._title.toLatin1().data()))));
        check = new QCheckBox;
        check->setEnabled(false);
        _testSummaryTree->setItemWidget(_functionTreeItem[i], 1, check);
    }
    _testSummaryTree->expandAll();
}

void CTestSummaryWindow::finishTestJob()
{
    int iRet = QMessageBox::question(this, "提示", "您确定完成本次测试，并记录到数据库？",
                                     QMessageBox::Yes, QMessageBox::No);

    if(iRet == QMessageBox::No)
    {
        //***删除新增加的记录 调用下面的代码
        QVector<testRecord> t = ((CApp*)qApp)->_meter->_tr;
        ((CApp*)qApp)->_db->deleteMeter(QString::number(static_cast<testRecord>(t.last())._id));
        //***删除本次选中的测试的记录  调用下面的代码
        //QString testid = qobject_cast<QTabWidget*>(parentWidget()->parentWidget())->widget(0)->property("testid").toString();
        //((CApp*)qApp)->_db->deleteMeter(testid);
    }
    else if (iRet == QMessageBox::Yes)
    {
        QMessageBox::question(this, "提示", "保存成功！",QMessageBox::Ok);
    }
}
