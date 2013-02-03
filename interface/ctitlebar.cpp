#include "ctitlebar.h"
#include "csystembutton.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QApplication>
#include <QMessageBox>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

CTitleBar::CTitleBar(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("311仪表检测控制软件"));
    QLabel *winTitle=new QLabel(tr("<b style='font:14px;color:#000049'>311仪表检测控制软件<b>"));
    QGraphicsDropShadowEffect *titleEffect = new QGraphicsDropShadowEffect;
    titleEffect->setBlurRadius(1);
    titleEffect->setColor(QColor("#fff"));
    titleEffect->setOffset(1,1);
    winTitle->setGraphicsEffect(titleEffect);
    m_minButton=new csystembutton((":/res/sys_button_min.png"),tr("最小化"));
    //m_maxButton=new csystembutton(":/res/sys_button_max.png",tr("最大化"));
    m_closeButton=new csystembutton(":/res/sys_button_close.png",tr("关闭"));

    QHBoxLayout *mainLayout=new QHBoxLayout;
    mainLayout->addWidget(winTitle,0,Qt::AlignVCenter);
    winTitle->setContentsMargins(5,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addStretch();
    mainLayout->addWidget(m_minButton);
    //mainLayout->addWidget(m_maxButton);
    mainLayout->addWidget(m_closeButton);

    setLayout(mainLayout);

    connect(m_minButton,SIGNAL(clicked()),this,SIGNAL(windowToMin()));
    //connect(m_maxButton,SIGNAL(clicked()),this,SIGNAL(maxshow()));
    connect(m_closeButton,SIGNAL(clicked()),this,SIGNAL(windowToClose()));

    setFixedHeight(25);
    m_bMouseDown=false;
    winTitle->installEventFilter(this);
    this->installEventFilter(this);
}

bool CTitleBar::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        m_bMouseDown = true;
        m_ptMouseDown =  static_cast<QMouseEvent*>(event)->pos() ;
        setCursor(Qt::SizeAllCursor);
    }
    else if(event->type() == QEvent::MouseMove)
    {
        if( m_bMouseDown)
        {
            static QWidget* parent = this->parentWidget();
            QPoint currentPos = parent->pos();
            currentPos.setX(currentPos.x() + static_cast<QMouseEvent*>(event)->x()-m_ptMouseDown.x());
            currentPos.setY(currentPos.y() + static_cast<QMouseEvent*>(event)->y()-m_ptMouseDown.y());
            parent->move(currentPos);
        }
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        if(m_bMouseDown)
            m_bMouseDown=false;
        setCursor(Qt::ArrowCursor);
    }
}
