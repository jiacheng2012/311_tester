#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QWidget>
#include <QtGui>
#include "ctestsettingwindow.h"
#include "ctestsummarywindow.h"
#include "bootloader/cbootloaderwindow.h"
#include "inittest/cinittestwindow.h"
#include "fulltest/cfulltestwindow.h"
#include "adjusttest/cadjusttestwindow.h"
#include "functiontest/cfunctiontestwindow.h"
#include "processing/ctestjob.h"
#include "../cobjectfactory.h"

class CTestWindow : public QWidget
{
    Q_OBJECT

private:
    CObjectFactory              _factory;
    QMap<QString, int>         _objectMap;

protected:
    virtual void closeEvent(QCloseEvent *);
protected:
    void timerEvent(QTimerEvent* event);

public:

public:
    CTestWindow(QWidget *parent = 0);
    ~CTestWindow();

    QTabBar          *groupBar;
    QVBoxLayout         *topLayout;
    QHBoxLayout         *upLayout;
    QTextEdit           *_logText;
    QLabel *_testTimer; //时间
    QLabel *_testorName;//测试员用户名
    QLabel *_testedMeterNO;//测试 仪表编号
    QLabel *_plcConnectStatus; //PLC 连接状态
    QLabel *_meterConnectStatus;//仪表板 连接状态
    QLabel *_dbConnectStatus;//数据库连接状态
    int                 _currentTabIndex;

signals:
    void updateTestJobSetting();
    void getFullTestData(QByteArray);
    void startFullTest();
    void comSetup();
protected:
    void keyReleaseEvent(QKeyEvent *event);
    bool eventFilter(QObject *, QEvent *);
private:

public slots:
    void showContextMenu(const QPoint&);
    void newMessage(QString msg, bool showOnScreen=0);
    void toTestTabPage(int page);
    void _232Setup();
    void _485Setup();
    void databaseSetup();
    void jobSetup(QString,QString,QString);
    void stop232();
    void stop485();

private slots:

};

#endif // TESTWINDOW_H
