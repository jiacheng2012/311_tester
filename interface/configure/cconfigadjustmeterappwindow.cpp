#include "cconfigadjustmeterappwindow.h"
#include "capp.h"

CConfigAdjustMeterAppWindow::CConfigAdjustMeterAppWindow(QWidget *parent) :
    QWidget(parent)
{
    //***UI CONPONENTS
    _accumulatedMileage = new QSpinBox; //累计里程
    _accumulatedMileage->setMaximum(999999999);
    _simpleMileage = new QSpinBox; //小计里程
    _simpleMileage->setMaximum(999999999);

    _ledlight = new QSpinBox; //背光亮度
    _ledlight->setMaximum(255);
    _lcdbacklight = new QSpinBox; //指示灯亮度
    _lcdbacklight->setMaximum(255);

    _lowspeedFanDiff = new QSpinBox; //低速风扇回差
    _lowspeedFanDiff->setMaximum(255);
    _highspeedFanDiff = new QSpinBox; //高速风扇回差
    _highspeedFanDiff->setMaximum(255);

    _warn_coolingWaterTemperature = new QSpinBox; //冷却水温报警点
    _warn_coolingWaterTemperature->setMaximum(255);
    _warn_oilpressure = new QSpinBox; //机油压力报警点
    _warn_oilpressure->setMaximum(255);
    _warn_oil = new QSpinBox; //燃油报警点
    _warn_oil->setMaximum(100);
    _warn_lowpressure = new QSpinBox; //气压低报警点
    _warn_lowpressure->setMaximum(255);

    _lowspeedFanInGasTemperature = new QSpinBox; //低速风扇进气温度参数
    _lowspeedFanInGasTemperature->setMaximum(255);
    _highspeedFanInGasTemperature = new QSpinBox; //高速风扇进气温度参数
    _highspeedFanInGasTemperature->setMaximum(255);
    _lowspeedFan_coolingWaterTemperature = new QSpinBox; //低速风扇冷却水温参数
    _lowspeedFan_coolingWaterTemperature->setMaximum(255);
    _highspeedFan_coolingWaterTemperature = new QSpinBox; //高速风扇冷却水温参数
    _highspeedFan_coolingWaterTemperature->setMaximum(255);

    _pulseCount = new QSpinBox; //脉冲计数
    _pulseCount->setMaximum(999999999);
    _engineWorkTime_Hour = new QSpinBox; //发动机工作时间(时)
    _engineWorkTime_Hour->setMaximum(999999999);
    _engineWorkTime_Minute = new QSpinBox; //发动机工作时间(分)
    _engineWorkTime_Minute->setMaximum(999999999);
    _engineWorkTime_Second = new QSpinBox; //发动机工作时间(秒)
    _engineWorkTime_Second->setMaximum(999999999);

    _accumulatedMileage->setObjectName("milage_total");
    _simpleMileage->setObjectName("milage_part");
    _warn_coolingWaterTemperature->setObjectName("w_cwater");
    _warn_oilpressure->setObjectName("w_lpressure");
    _warn_oil->setObjectName("w_opressure");
    _warn_lowpressure->setObjectName("w_pressurel");
    _lcdbacklight->setObjectName("backlight");
    _ledlight->setObjectName("ledlight");
    _lowspeedFanInGasTemperature->setObjectName("fan_lspeed_temp");
    _highspeedFanInGasTemperature->setObjectName("fan_hspeed_temp");
    _lowspeedFan_coolingWaterTemperature->setObjectName("fan_lspeed_wtemp");
    _highspeedFan_coolingWaterTemperature->setObjectName("fan_hspeed_wtemp");
    _lowspeedFanDiff->setObjectName("fan_lspeed_diff");
    _highspeedFanDiff->setObjectName("fan_hspeed_diff");
    _pulseCount->setObjectName("milage_pulse");
    _engineWorkTime_Hour->setObjectName("engine_hour");
    _engineWorkTime_Minute->setObjectName("engine_minute");
    _engineWorkTime_Second->setObjectName("engine_second");

    /*QBrush brushToolTipBase(Qt::yellow);
    QBrush brushToolTipText(Qt::darkBlue);
    QPalette palette = QToolTip::palette();
    palette.setBrush(QPalette::ToolTipBase, brushToolTipBase);
    palette.setBrush(QPalette::ToolTipText, brushToolTipText);
    palette.setColor(QPalette::ToolTipBase, Qt::yellow);
    palette.setColor(QPalette::ToolTipText, Qt::darkBlue);
    QToolTip::setPalette(palette);QFont serifFont("Times", 14, QFont::Bold);
    QToolTip::setFont(serifFont);*/
    _readButton = new QPushButton(tr("打开应用参数配置 (Ctrl + O)"));
    _readButton->setStyleSheet("min-width:280px;min-height:48px;color:#333;font:bold 16px;");
    _saveButton = new QPushButton(tr("保存应用参数配置 (Ctrl + S)"));
    _saveButton->setStyleSheet("min-width:280px;min-height:48px;color:#333;font:bold 16px;");
    //***Layout
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QLabel* title = new QLabel(tr("应用参数配置"));
    title->setStyleSheet("background:#EFF7FD;color:#1E50A2;font:bold 16px;min-width:300px;max-height:30px;");
    title->setAlignment(Qt::AlignCenter);
    QHBoxLayout *group1_h1 = new QHBoxLayout;
    QGroupBox *group1 = new QGroupBox(tr("命令参数"));
    group1->setFixedHeight(450);
    QVBoxLayout *group1_vlayout = new QVBoxLayout(group1);
    QGroupBox *group1_1 = new QGroupBox(tr("里程参数"));
    QGridLayout *grid1_1 = new QGridLayout(group1_1);
    grid1_1->addWidget(new QLabel(tr("累计里程(m)"),0,0));
    grid1_1->addWidget(new QLabel(tr("小计里程(m)"),0,0));
    grid1_1->addWidget(_accumulatedMileage,0,1);
    grid1_1->addWidget(_simpleMileage,1,1);
    QGroupBox *group1_2 = new QGroupBox(tr("LED亮度"));
    QGridLayout *grid1_2 = new QGridLayout(group1_2);
    grid1_2->addWidget(new QLabel(tr("背光亮度")),0,0);
    grid1_2->addWidget(new QLabel(tr("指示灯亮度")),1,0);
    grid1_2->addWidget(_ledlight,0,1);
    grid1_2->addWidget(_lcdbacklight,1,1);
    QGroupBox *group1_3 = new QGroupBox(tr("风扇回差"));
    QGridLayout *grid1_3 = new QGridLayout(group1_3);
    grid1_3->addWidget(new QLabel(tr("低速风扇回差")),0,0);
    grid1_3->addWidget(new QLabel(tr("高速风扇回差")),1,0);
    grid1_3->addWidget(_lowspeedFanDiff,0,1);
    grid1_3->addWidget(_highspeedFanDiff,1,1);
    group1_h1->addWidget(group1_1);
    group1_h1->addWidget(group1_2);
    group1_h1->addWidget(group1_3);

    QHBoxLayout *group1_h2 = new QHBoxLayout;
    QGroupBox *group2_1 = new QGroupBox(tr("报警点"));
    QGridLayout *grid2_1 = new QGridLayout(group2_1);
    grid2_1->addWidget(new QLabel(tr("冷却水温报警点")),0,0);
    grid2_1->addWidget(new QLabel(tr("机油压力报警点")),1,0);
    grid2_1->addWidget(new QLabel(tr("燃油报警点")),2,0);
    grid2_1->addWidget(new QLabel(tr("气压低报警点")),3,0);
    grid2_1->addWidget(_warn_coolingWaterTemperature,0,1);
    grid2_1->addWidget(_warn_oilpressure,1,1);
    grid2_1->addWidget(_warn_oil,2,1);
    grid2_1->addWidget(_warn_lowpressure,3,1);
    grid2_1->addWidget(new QLabel(tr("℃")),0,2);
    grid2_1->addWidget(new QLabel(tr("kpa")),1,2);
    grid2_1->addWidget(new QLabel(tr("%")),2,2);
    grid2_1->addWidget(new QLabel(tr("kpa")),3,2);

    QGroupBox *group2_2 = new QGroupBox(tr("温度参数"));
    QGridLayout *grid2_2 = new QGridLayout(group2_2);
    grid2_2->addWidget(new QLabel(tr("低速风扇进气温度参数")),0,0);
    grid2_2->addWidget(new QLabel(tr("高速风扇进气温度参数")),1,0);
    grid2_2->addWidget(new QLabel(tr("低速风扇冷却水温参数")),2,0);
    grid2_2->addWidget(new QLabel(tr("高速风扇冷却水温参数")),3,0);
    grid2_2->addWidget(_lowspeedFanInGasTemperature,0,1);
    grid2_2->addWidget(_highspeedFanInGasTemperature,1,1);
    grid2_2->addWidget(_lowspeedFan_coolingWaterTemperature,2,1);
    grid2_2->addWidget(_highspeedFan_coolingWaterTemperature,3,1);
    grid2_2->addWidget(new QLabel(tr("℃")),0,2);
    grid2_2->addWidget(new QLabel(tr("℃")),1,2);
    grid2_2->addWidget(new QLabel(tr("℃")),2,2);
    grid2_2->addWidget(new QLabel(tr("℃")),3,2);

    QGroupBox *group2_3 = new QGroupBox(tr("其它参数"));
    QGridLayout *grid2_3 = new QGridLayout(group2_3);
    grid2_3->addWidget(new QLabel(tr("脉冲计数")),0,0);
    grid2_3->addWidget(new QLabel(tr("发动机工作时间")),1,0);

    grid2_3->addWidget(_pulseCount,0,1);
    grid2_3->addWidget(_engineWorkTime_Hour,1,1);
    grid2_3->addWidget(_engineWorkTime_Minute,2,1);
    grid2_3->addWidget(_engineWorkTime_Second,3,1);
    grid2_3->addWidget(new QLabel(tr("个")),0,2);
    grid2_3->addWidget(new QLabel(tr("时")),1,2);
    grid2_3->addWidget(new QLabel(tr("分")),2,2);
    grid2_3->addWidget(new QLabel(tr("秒")),3,2);

    group1_h2->addWidget(group2_1);
    group1_h2->addWidget(group2_2);
    group1_h2->addWidget(group2_3);
    group1_vlayout->addLayout(group1_h1);
    group1_vlayout->addLayout(group1_h2);

    QHBoxLayout* bottom = new QHBoxLayout;
    bottom->addWidget(_readButton,0,Qt::AlignHCenter);
    bottom->addWidget(_saveButton,0,Qt::AlignHCenter);
    bottom->setSpacing(20);
    //***新加
    mainVLayout->addWidget(title,0,Qt::AlignHCenter);
    mainVLayout->addWidget(group1,1,Qt::AlignHCenter);
    mainVLayout->addLayout(bottom,0);
    mainVLayout->setAlignment(bottom,Qt::AlignHCenter);
    setLayout(mainVLayout);
    //***signals
    connect(_readButton,SIGNAL(clicked()),this,SLOT(on__readButton_clicked()));
    connect(_saveButton,SIGNAL(clicked()),this,SLOT(on__saveButton_clicked()));
}

void CConfigAdjustMeterAppWindow::on__saveButton_clicked()
{
    QString filename;
    QFileDialog dlg(this,"保存配置",QDir::currentPath(),"配置文件 (*.ini)");
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setFileMode(QFileDialog::AnyFile);

    if(dlg.exec() == QFileDialog::FileName)
    {
        filename = dlg.selectedFiles().first();
        if(!filename.endsWith(".ini"))
        {
            filename.append(".ini");
        }
        QSettings *sIniSetting = new QSettings(filename, QSettings::IniFormat);
        sIniSetting->setIniCodec("UTF-8");

        sIniSetting->setValue("function_param/milage_total",_accumulatedMileage->value());
        sIniSetting->setValue("function_param/milage_part",_simpleMileage->value());
        sIniSetting->setValue("function_param/ledlight",_ledlight->value());
        sIniSetting->setValue("function_param/backlight",_lcdbacklight->value());
        sIniSetting->setValue("function_param/fan_lspeed_diff",_lowspeedFanDiff->value());
        sIniSetting->setValue("function_param/fan_hspeed_diff",_highspeedFanDiff->value());

        sIniSetting->setValue("function_param/w_cwater",_warn_coolingWaterTemperature->value());
        sIniSetting->setValue("function_param/w_lpressure",_warn_oilpressure->value());
        sIniSetting->setValue("function_param/w_opressure",_warn_oil->value());
        sIniSetting->setValue("function_param/w_pressurel",_warn_lowpressure->value());

        sIniSetting->setValue("function_param/fan_lspeed_temp",_lowspeedFanInGasTemperature->value());
        sIniSetting->setValue("function_param/fan_hspeed_temp",_highspeedFanInGasTemperature->value());
        sIniSetting->setValue("function_param/fan_lspeed_wtemp",_lowspeedFan_coolingWaterTemperature->value());
        sIniSetting->setValue("function_param/fan_hspeed_wtemp",_highspeedFan_coolingWaterTemperature->value());

        sIniSetting->setValue("function_param/milage_pulse",_pulseCount->value());
        sIniSetting->setValue("function_param/engine_hour",_engineWorkTime_Hour->value());
        sIniSetting->setValue("function_param/engine_minute",_engineWorkTime_Minute->value());
        sIniSetting->setValue("function_param/engine_second",_engineWorkTime_Second->value());

        sIniSetting->sync();
        delete sIniSetting;
        QMessageBox::information(this, tr("提示"), tr("保存配置文件成功！"),QMessageBox::Ok);
    }
}

void CConfigAdjustMeterAppWindow::on__readButton_clicked()
{
    QString filename;
    QFileDialog dlg(this,"读取配置",QDir::currentPath(),"配置文件 (*.ini)");
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);

    if(dlg.exec() == QFileDialog::FileName)
    {
        filename = dlg.selectedFiles().first();

        QSettings *sIniSetting = new QSettings(filename, QSettings::IniFormat);
        sIniSetting->setIniCodec("UTF-8");
        QStringList strList;
        int validCount = 0;
        strList << "milage_total" << "milage_part" << "w_cwater" << "w_lpressure" << "w_opressure" << "w_pressurel"
                << "backlight" << "ledlight" << "fan_lspeed_temp" << "fan_hspeed_temp" << "fan_lspeed_wtemp" << "fan_hspeed_wtemp"
                << "fan_lspeed_diff" << "fan_hspeed_diff" << "milage_pulse" << "engine_hour" << "engine_minute" << "engine_second";
        //***导入仪表应用参数文件检查
        foreach(QString str,strList)
        {

            if(sIniSetting->value("function_param/" + str).isValid())
            {
                //***有一个合法自动，暂定为合法配置文件
                findChildren<QSpinBox* >(str).first()->setValue(sIniSetting->value("function_param/" + str).toInt());
                ++ validCount;
            }
            else
            {
                findChildren<QSpinBox* >(str).first()->setValue(0);
                //***至少有一个字段缺失则不合法配置文件
            }
        }
        if(validCount == 0) //***没有仪表应用配置相关属性
        {
            //QMessageBox::information(this, tr("提示"), tr("没有仪表应用配置相关属性"),QMessageBox::Ok);
        }
        else if(validCount != strList.size())//***缺失相关属性
        {
            QMessageBox::information(this, tr("提示"), tr("该配置文件缺少若干仪表应用参数配置的属性，已经按默认方式初始化！"),QMessageBox::Ok);
        }
        else
        {
            QMessageBox::information(this, tr("提示"), tr("导入仪表应用参数配置成功！"),QMessageBox::Ok);
        }
        sIniSetting->sync();
        delete sIniSetting;
    }
}

void CConfigAdjustMeterAppWindow::keyPressEvent(QKeyEvent *)
{

}

void CConfigAdjustMeterAppWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {

        }
        else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_O)
        {
            on__readButton_clicked();
        }
        else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S)
        {
            on__saveButton_clicked();
        }
    }
}
