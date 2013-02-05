#define _WIN32_WINDOWS 0x0410
#define _WIN32_WINNT  0x0500
#include <windows.h>
#include <QMessageBox>
#include <QMenuBar>
#include "CMainWindow.h"
#include "capp.h"


CMainWindow::CMainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    setWindowTitle(tr("311仪表检测控制软件"));
    //***UI Component
    page0 = new CLoginWindow(this);
    page1 = new CSelWindow(this);
    page2 = new CTestWindow(this);
    page3 = new CConfigWindow(this);
    page4 = new CDatabaseWindow(this);

    _mTitleBar = new CTitleBar;
    QVBoxLayout* mainVLayout = new QVBoxLayout;
    mainVLayout->setSpacing(0);
    mainVLayout->setContentsMargins(0,0,0,0);
    mainVLayout->setMargin(0);

    stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(page0);
    stackedLayout->addWidget(page1);
    stackedLayout->addWidget(page2);
    stackedLayout->addWidget(page3);
    stackedLayout->addWidget(page4);

    stackedLayout->setCurrentIndex(0);
    mainVLayout->addWidget(_mTitleBar);
    mainVLayout->addLayout(stackedLayout);
    CreatTrayMenu();
    CreatTrayIcon();
    setLayout(mainVLayout);
    minimizeWindow();
    connect(_mTitleBar,SIGNAL(windowToMin()),this,SLOT(showMinimized()));
    connect(_mTitleBar,SIGNAL(windowToClose()),this,SLOT(close()));
}

CMainWindow::~CMainWindow()
{

}

void CMainWindow::loginButtonClicked()
{
    stackedLayout->setCurrentIndex(1);
    _mTrayIcon->showMessage(windowTitle(), "登录成功！", QSystemTrayIcon::Information, 1);
}

void CMainWindow::testButtonClicked()
{
    bool bNeedNewWindow = true;
    int  index = 0;
    for(; index < stackedLayout->count(); index++)
    {
        if(QString::compare(stackedLayout->widget(index)->metaObject()->className(),"CTestWindow") == 0)
        {
            bNeedNewWindow = false;
            break;
        }
    }
    if(bNeedNewWindow)
    {
        page2 = new CTestWindow(this);
        stackedLayout->addWidget(page2);
        stackedLayout->setCurrentWidget(page2);
    }
    else
    {
        stackedLayout->setCurrentIndex(index);
    }
    //maximizeWindow();
    resizeAndCenterWindow(900,720);
}

void CMainWindow::configButtonClicked()
{
    bool bNeedNewWindow = true;
    int  index = 0;
    for(; index < stackedLayout->count(); index++)
    {
        if(QString::compare(stackedLayout->widget(index)->metaObject()->className(),"CConfigWindow") == 0)
        {
            bNeedNewWindow = false;
            break;
        }
    }
    if(bNeedNewWindow)
    {
        page3 = new CConfigWindow(this);
        stackedLayout->addWidget(page3);
        stackedLayout->setCurrentWidget(page3);
    }
    else
    {
        stackedLayout->setCurrentIndex(index);
    }
    resizeAndCenterWindow(900,720);
}

void CMainWindow::databaseButtonClicked()
{
    bool bNeedNewWindow = true;
    int  index = 0;
    for(; index < stackedLayout->count(); index++)
    {
        if(QString::compare(stackedLayout->widget(index)->metaObject()->className(),"CDatabaseWindow") == 0)
        {
            bNeedNewWindow = false;
            break;
        }
    }
    if(bNeedNewWindow)
    {
        page4 = new CDatabaseWindow(this);
        stackedLayout->addWidget(page4);
        stackedLayout->setCurrentWidget(page4);
    }
    else
    {
        stackedLayout->setCurrentIndex(index);
    }
    resizeAndCenterWindow(900,720);
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
    QWidget* widget = stackedLayout->currentWidget();
    if(widget == page2)
    {
        int iRet = QMessageBox::question(this, "关闭提示", "您确定退出产品检测吗？\n请确保您需要的测试项已经完成！",
                                         QMessageBox::Yes, QMessageBox::No);

        if(iRet == QMessageBox::No)
        {
            event->ignore();
            return;
        }
        else if (iRet == QMessageBox::Yes)
        {
            stackedLayout->removeWidget(widget);
            widget->deleteLater();
            stackedLayout->setCurrentIndex(1);
            minimizeWindow();
            event->ignore();
            ((CApp*)qApp)->deleteJob();
            return;
        }
    }
    else if(widget != page0 && widget != page1)
    {
        int iRet = QMessageBox::question(this, "关闭提示", "您确定退出当前页面吗?",
                                         QMessageBox::Yes, QMessageBox::No);

        if(iRet == QMessageBox::No)
        {
            event->ignore();
            return;
        }
        else if (iRet == QMessageBox::Yes)
        {
            stackedLayout->removeWidget(widget);
            widget->deleteLater();
            stackedLayout->setCurrentIndex(1);
            minimizeWindow();
            event->ignore();
            return;
        }
    }
    else
    {
        event->accept();
        _mTrayIcon->hide();
        _mTrayIcon->deleteLater();
    }

    QWidget::closeEvent(event);
}

void CMainWindow::animateWindow(bool bshow)
{
    QRect qrcFrom = this->geometry();
    QRect qrcTo = this->_mTrayIcon->geometry();
    RECT rcFrom = {qrcFrom.left(),qrcFrom.top(),qrcFrom.right(),qrcFrom.bottom()};
    RECT rcTo = {qrcTo.left(),qrcTo.top(),qrcTo.right(),qrcTo.bottom()};
    if(bshow)
        DrawAnimatedRects(this->winId(),IDANI_CAPTION,&rcFrom,&rcTo);
    else
        DrawAnimatedRects(this->winId(),IDANI_CAPTION,&rcTo,&rcFrom);
}

void CMainWindow::minimizeWindow()
{
    setFixedSize(QSize(360, 280));
    setGeometry((QApplication::desktop()->availableGeometry().width()-360)/2,
                (QApplication::desktop()->availableGeometry().height()-280)/2,
                360, 280);
}

void CMainWindow::maximizeWindow()
{
    setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    setMinimumSize(960,720);
    setGeometry((QApplication::desktop()->availableGeometry().width()-width())/2,
                (QApplication::desktop()->availableGeometry().height()-height())/2,
                width(), height());
    showMaximized();
}

void CMainWindow::resizeAndCenterWindow(int width, int height)
{
    setFixedSize(width, height);
    Q_UNUSED(width)
    Q_UNUSED(height)
    setGeometry((QApplication::desktop()->availableGeometry().width()- this->width())/2,
                (QApplication::desktop()->availableGeometry().height()-this->height())/2,
                this->width(), this->height());
}

void CMainWindow::paintEvent(QPaintEvent *)
{
    /*QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);*/
    QBitmap bitmap(this->size());
    bitmap.fill();
    QPainter painter(&bitmap);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRoundedRect(bitmap.rect(),5,5);
    setMask(bitmap);
}

void CMainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::Context:
        break;
    case QSystemTrayIcon::DoubleClick:

        if(this->isHidden())
        {
            animateWindow(false);
            show();
            SwitchToThisWindow(this->winId(),true);
        }
        else
        {
            animateWindow(true);
            QTimer::singleShot(0, this, SLOT(hide()));
        }
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}

bool CMainWindow::winEvent(MSG *message, long *result)
{
    if(message->message == WM_SIZE)
    {
        if(message->wParam == SIZE_MINIMIZED)
        {
            if(sender() != 0)
            {
                animateWindow(true);
                QTimer::singleShot(0, this, SLOT(hide()));

                static bool showonce = true;
                if(showonce)
                {
                    _mTrayIcon->showMessage(windowTitle(),"隐藏至托盘，双击可以恢复显示！");
                    showonce = false;
                }
                return true;
            }
        }
    }

    QWidget::winEvent(message, result);
}

void CMainWindow::quitMenuClicked()
{
    show();
    SwitchToThisWindow(this->winId(),true);
    QTimer::singleShot(0, this, SLOT(close()));
}

void CMainWindow::CreatTrayMenu()
{
    _mQuitAction = new QAction(tr("退出 (&Q)"), this);
    connect(_mQuitAction, SIGNAL(triggered()), this, SLOT(quitMenuClicked()));
}

void CMainWindow::CreatTrayIcon()
{
    QIcon icon = QIcon(":/res/logo.ico");

    _mTrayIcon = new QSystemTrayIcon(this);
    _mTrayIcon->setIcon(icon);
    _mTrayIcon->setToolTip("311仪表检测控制");

    _mMenu = new QMenu(this);
    //_mMenu->addSeparator();
    _mMenu->addAction(_mQuitAction);
    _mTrayIcon->setContextMenu(_mMenu);
    connect(_mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    _mTrayIcon->show();
}
