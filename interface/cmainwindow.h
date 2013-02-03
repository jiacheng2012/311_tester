#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtGui>
#include "csystembutton.h"
#include "ctitlebar.h"
#include "cselwindow.h"
#include "cloginwindow.h"
#include "test/ctestwindow.h"
#include "database/cdatabasewindow.h"
#include "configure/cconfigwindow.h"
#include "database/cuser.h"

class CMainWindow : public QWidget
{
    Q_OBJECT
    
public:
    CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

    CLoginWindow* page0;
    CSelWindow* page1;
    CTestWindow* page2;
    CConfigWindow* page3;
    CDatabaseWindow* page4;

    QStackedLayout *stackedLayout;
    QSystemTrayIcon *_mTrayIcon;

protected:
    virtual void closeEvent(QCloseEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual bool winEvent(MSG *message, long *result);
private:
    void maximizeWindow();
    void minimizeWindow();
    void CreatTrayMenu(); //创建托盘菜单
    void CreatTrayIcon(); //创建托盘
    void animateWindow(bool bshow);
private:
    CTitleBar* _mTitleBar;
    QMenu *_mMenu;
    QAction *_mQuitAction;

public slots:
    void quitMenuClicked();
    void loginButtonClicked();
    void testButtonClicked();
    void configButtonClicked();
    void databaseButtonClicked();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
public:
    void resizeAndCenterWindow(int width, int height);
};

#endif // MAINWINDOW_H
