#include "cdatabasewindow.h"

CDatabaseWindow::CDatabaseWindow(QWidget *parent)
    : QWidget(parent)
{
    //***UI Component
    topLayout = new QVBoxLayout;

    //***Tab Component
    group = new QTabWidget();
    QWidget* op1 = new QWidget();
    group->addTab(op1,tr("作业设置"));

    QWidget* op2 = new QWidget();
    group->addTab(op2,tr("程序下载"));

    QWidget* op3 = new QWidget();
    group->addTab(op3,tr("单板初检"));

    topLayout->addWidget(group);
    setLayout(topLayout);
    //central widget
    //    QespTest* qespTest = new QespTest();

    //    firstLayout->addWidget(qespTest);
    //***

    //***Signal
}

CDatabaseWindow::~CDatabaseWindow()
{

}

