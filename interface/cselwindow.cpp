#include <QMessageBox>
#include <QMenuBar>
#include "cselwindow.h"

CSelWindow::CSelWindow(QWidget *parent)
    : QWidget(parent)
{
    //***UI Component
    topLayout = new QVBoxLayout;
    QGroupBox *selectGroup = new QGroupBox(tr("选择操作模式"));
    QVBoxLayout* groupVLayout = new QVBoxLayout(selectGroup);
    testButton = new QPushButton();
    configButton = new QPushButton();
    databaseButton = new QPushButton();
    testButton->setProperty("selwindow-btn","true");
    configButton->setProperty("selwindow-btn","true");
    databaseButton->setProperty("selwindow-btn","true");
    testButton->setText(tr("产品检测"));
    configButton->setText(tr("调试硬件配置"));
    databaseButton->setText(tr("数据库管理"));

    groupVLayout->addWidget(testButton);
    groupVLayout->addWidget(configButton);
    groupVLayout->addWidget(databaseButton);
    groupVLayout->setContentsMargins(40,10,40,10);
    topLayout->addWidget(selectGroup);
    topLayout->setContentsMargins(35,20,35,20);
    setLayout(topLayout);

    //***Signal
    connect(testButton,SIGNAL(clicked()),this->parent(),SLOT(testButtonClicked()));
    connect(configButton,SIGNAL(clicked()),this->parent(),SLOT(configButtonClicked()));
    connect(databaseButton,SIGNAL(clicked()),this->parent(),SLOT(databaseButtonClicked()));
}

CSelWindow::~CSelWindow()
{

}
