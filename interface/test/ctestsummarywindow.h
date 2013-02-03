#ifndef CTESTSUMMARYWINDOW_H
#define CTESTSUMMARYWINDOW_H

#include <QWidget>
#include <QtGui>

#define     ALL_TEST_COUNT       19
#define     INIT_TEST_COUNT      4
#define     FULL_TEST_COUNT      13
#define     ADJUST_TEST_COUNT    5
#define     FUNCTION_TEST_COUNT  9

typedef struct tagTestItem
{
    QString _title;

}TestItem,*PTestItem;

const  TestItem initTArray[INIT_TEST_COUNT + 1] = {
    {"仪表单板检测"},{"短路检测"},{"电压点检测"},{"电阻点检测"},{"Bootloader下载"}
};
const   TestItem fullTArray[FULL_TEST_COUNT + 1] = {
    {"仪表硬件全检"},{"指示灯测试"},{"液晶屏背光测试"},{"按键测试"},{"存储测试"},{"指示灯测试"},{"自供电测试"},
    {"仪表电源电压采样测试"},{"步进电机测试"},{"CAN测试"},{"32路IO输入测试"},{"8路IO输出测试"},{"模拟量测试"},{"频率量测试"}
};
const  TestItem adjustTArray[ADJUST_TEST_COUNT + 1] = {
    {"仪表调校"},{"车速表调校"},{"转速表调校"},{"燃油表调校"},{"水温表调校"},{"模拟量调校"}
};
const  TestItem functionTArray[FUNCTION_TEST_COUNT + 1] = {
    {"仪表功能全检"},{"指示灯测试"},{"车速表测试"},{"转速表测试"},{"燃油表测试"},{"水温表测试"},{"气压测试"},
    {"机油压力测试"},{"进气温度测试"},{"整车电压测试"}
};

class CTestSummaryWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CTestSummaryWindow(QWidget *parent = 0);

signals:

public slots:
    void updateTestSummary(); /*刷新测试结果*/
    void finishTestJob();
private:
    QTreeWidget* _testSummaryTree;
    QTreeWidgetItem *_initTreeItem[INIT_TEST_COUNT + 1], *_fullTreeItem[FULL_TEST_COUNT + 1],
                    *_adjustTreeItem[ADJUST_TEST_COUNT + 1], *_functionTreeItem[FUNCTION_TEST_COUNT + 1] ;
};

#endif
