#include "cconfigwindow.h"

CConfigWindow::CConfigWindow(QWidget *parent)
    : QWidget(parent)
{
    //***UI Component
    QVBoxLayout *topLayout = new QVBoxLayout;

    //***Tab Component
    group = new QTabWidget();

    _mMeterAppWnd = new CConfigAdjustMeterAppWindow;
    _mChannelWnd = new CConfigChannelWindow;
    _mAdjustParmsWnd = new CConfigAdjustParmsWindow;

    group->addTab(_mMeterAppWnd,tr("仪表应用参数配置"));
    group->addTab(_mChannelWnd,tr("通道参数配置"));
    group->addTab(_mAdjustParmsWnd,tr("调校参数配置"));
    group->setFocusPolicy(Qt::NoFocus);

    topLayout->addWidget(group);
    setLayout(topLayout);
}

CConfigWindow::~CConfigWindow()
{

}
/*
void CConfigWindow::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}*/
