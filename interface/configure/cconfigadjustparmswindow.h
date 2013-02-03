#ifndef CONFIG_ADJUSTPARMS_WINDOW_H
#define CONFIG_ADJUSTPARMS_WINDOW_H

#include <QWidget>
#include <QtGui>

class CConfigAdjustParmsWindow : public QWidget
{
    Q_OBJECT

public:
    CConfigAdjustParmsWindow(QWidget *parent = 0);
    ~CConfigAdjustParmsWindow();
private:

    QTabWidget *        _parmsTabs; //tab控件，包含车速，转速,燃油，水温，模拟量等子页
    QMap<QString,int>   _analogTypeNameMap;
    QWidget*            _parmsTabContent1;
    QLineEdit*          _adjustPoints1;
    QPushButton*        _adjustPointsSetButton1;
    QVector<QSpinBox*>  _adjustValue1;
    QVector<QSpinBox*>  _adjustAmount1;

    QWidget*            _parmsTabContent2;
    QLineEdit*          _adjustPoints2;
    QPushButton*        _adjustPointsSetButton2;
    QVector<QSpinBox*>  _adjustValue2;
    QVector<QSpinBox*>  _adjustAmount2;

    QWidget*            _parmsTabContent3;
    QLineEdit*          _adjustPoints3;
    QPushButton*        _adjustPointsSetButton3;
    QVector<QSpinBox*>  _adjustValue3;
    QVector<QSpinBox*>  _adjustAmount3;

    QWidget*            _parmsTabContent4;
    QLineEdit*          _adjustPoints4;
    QPushButton*        _adjustPointsSetButton4;
    QVector<QSpinBox*>  _adjustValue4;
    QVector<QSpinBox*>  _adjustAmount4;
    //***模拟量调校参数
    QWidget*            _parmsTabContent5;
    QSpinBox*           _analogChannelNO; //模拟量通道号0-13
    QComboBox*           _analogChannelType; //0～5就是电流，6～8是电阻，9，10是电压，11～13是功率
    QLineEdit*          _adjustPoints5;
    QListWidget*         _analogList; //已经设置的模拟量通道列表
    QStackedLayout*      _analogWndsStackLayout;
    QPushButton*        _adjustPointsSetButton5;

    QPushButton*       _readButton;
    QPushButton*       _saveButton;

protected:
    void keyReleaseEvent(QKeyEvent *);
private slots:
    void showAdjustSettingControls();   //【车速，转速,燃油，水温】 通用
    void showAdjustSettingControls1();  //【模拟量调校】参数设置 界面与其他不同
    void analogChannelChanged(int);
    void analogListClicked(QListWidgetItem*);
    void analogListDoubleClicked(QListWidgetItem*);
    void    on__readButton_clicked();
    void    on__saveButton_clicked();
};

#endif // CONFIG_ADJUSTPARMS_WINDOW_H
