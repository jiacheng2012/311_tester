#ifndef CTESTSETTINGWINDOW_H
#define CTESTSETTINGWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"
#include "configure/cadjustconfig.h"

class CTestWindow;
class CTestSummaryWindow;

class CTestSettingWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CTestSettingWindow(QWidget *parent = 0);
    CTestWindow* getParent();

signals:
    void toTestTabPage(int);
protected:
    virtual void paintEvent(QPaintEvent *);
    void keyReleaseEvent(QKeyEvent *event);
public slots:
    void idButtonClicked();
    void updateTestJobSetting();
    void listDoubleClicked(int,int);
    void okSetButtonClicked();
    void meterButtonClicked();
    void adjustButtonClicked();

public:
    //***连接PLC
    QComboBox       *_plcPortNumber;//PLC端口号
    QComboBox       *_plcBaudRate;//PLC波特率
    QComboBox       *_plcDataBit;//PLC数据位parity check
    QComboBox       *_plcParityCheck;//PLC奇偶校验
    QComboBox       *_plcStopBit;//PLC停止位
    QComboBox       *_plcDataFlowControl;//PLC数据流控制

    //***连接仪表板
    QComboBox       *_meterPortNumber;//仪表板端口号
    QComboBox       *_meterBaudRate;//仪表板波特率
    QComboBox       *_meterDataBit;//仪表板数据位
    QComboBox       *_meterParityCheck;//仪表板奇偶校验
    QComboBox       *_meterStopBit;//仪表板停止位
    QComboBox       *_meterDataFlowControl;//仪表板数据流控制

    QPushButton     *_okSetButton;

    QGroupBox       *_firstGroup,*_secondGroup,*_thirdGroup;
    QPushButton     *_meterButton,*_adjustButton;
    QLineEdit       *_idText,*_meterFile,*_adjustFile;
    QLabel          *_resultText;
    QVBoxLayout     *topLayout;
    QPushButton     *_idButton,*_scanButton,*_testButton;
    QTableWidget    *_testList;
    //QTreeWidget     *_testDetails;

    CAdjustConfig       *_aconfig;
    CMeterConfig        *_mconfig;

    QLabel              *_statusLabel_1;

    //QCheckBox       *c1[INIT_TEST_NUM+1],*c2[FULL_TEST_NUM+1],*c3[ADJUST_TEST_NUM+1],*c4[FUNCTION_TEST_NUM+1];
    //QTreeWidgetItem *i1[INIT_TEST_NUM+1],*i2[FULL_TEST_NUM+1],*i3[ADJUST_TEST_NUM+1],*i4[FUNCTION_TEST_NUM+1];
};

#endif
