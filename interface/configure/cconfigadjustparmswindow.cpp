#include "cconfigwindow.h"
#include "capp.h"

CConfigAdjustParmsWindow::CConfigAdjustParmsWindow(QWidget *parent)
    : QWidget(parent)
{
    //***Gloabl UI Component
    _analogChannelNO = new QSpinBox;
    _analogChannelNO->setMaximum(13);
    _analogChannelType = new QComboBox;
    _analogChannelType->setEnabled(false);
    _analogTypeNameMap["电流"] = 0;
    _analogTypeNameMap["电阻"] = 1;
    _analogTypeNameMap["电压"] = 2;
    _analogTypeNameMap["功率"] = 3;
    _analogChannelType->addItems(QStringList()<<"电流"<<"电阻"<<"电压"<<"功率");
    _analogChannelType->setEditText("电流");
    _analogWndsStackLayout = new QStackedLayout;
    _analogList = new QListWidget;
    _analogList->setSortingEnabled(true);
    _analogList->setStyleSheet("max-width:110px;");
    _readButton = new QPushButton(tr("打开应用参数配置 (Ctrl + O)"));
    _readButton->setStyleSheet("min-width:280px;min-height:48px;color:#333;font:bold 16px;");
    _saveButton = new QPushButton(tr("保存应用参数配置 (Ctrl + S)"));
    _saveButton->setStyleSheet("min-width:280px;min-height:48px;color:#333;font:bold 16px;");

    //***【车速表】调校参数 设定 tab子窗口
    _parmsTabContent1 = new QWidget;
    _adjustPoints1 = new QLineEdit;
    _adjustPointsSetButton1 = new QPushButton(tr("确定"));

    QVBoxLayout *groupVLay1 = new QVBoxLayout;
    QHBoxLayout *groupHLay1 = new QHBoxLayout;
    groupHLay1->addWidget(new QLabel(tr("车速表校正点数：")));
    groupHLay1->addWidget(_adjustPoints1);
    groupHLay1->addWidget(_adjustPointsSetButton1);
    groupVLay1->addLayout(groupHLay1);
    _parmsTabContent1->setLayout(groupVLay1);

    //***【转速表】调校参数 设定 tab子窗口
    _parmsTabContent2 = new QWidget;
    _adjustPoints2 = new QLineEdit;
    _adjustPointsSetButton2 = new QPushButton(tr("确定"));

    QVBoxLayout *groupVLay2 = new QVBoxLayout;
    QHBoxLayout *groupHLay2 = new QHBoxLayout;
    groupHLay2->addWidget(new QLabel(tr("转速表校正点数：")));
    groupHLay2->addWidget(_adjustPoints2);
    groupHLay2->addWidget(_adjustPointsSetButton2);
    groupVLay2->addLayout(groupHLay2);
    _parmsTabContent2->setLayout(groupVLay2);

    //***【燃油表】调校参数 设定 tab子窗口
    _parmsTabContent3 = new QWidget;
    _adjustPoints3 = new QLineEdit;
    _adjustPointsSetButton3 = new QPushButton(tr("确定"));

    QVBoxLayout *groupVLay3 = new QVBoxLayout;
    QHBoxLayout *groupHLay3 = new QHBoxLayout;
    groupHLay3->addWidget(new QLabel(tr("燃油表校正点数：")));
    groupHLay3->addWidget(_adjustPoints3);
    groupHLay3->addWidget(_adjustPointsSetButton3);
    groupVLay3->addLayout(groupHLay3);
    _parmsTabContent3->setLayout(groupVLay3);

    //***【水温表】调校参数 设定 tab子窗口
    _parmsTabContent4 = new QWidget;
    _adjustPoints4 = new QLineEdit;
    _adjustPointsSetButton4 = new QPushButton(tr("确定"));

    QVBoxLayout *groupVLay4 = new QVBoxLayout;
    QHBoxLayout *groupHLay4 = new QHBoxLayout;
    groupHLay4->addWidget(new QLabel(tr("水温表校正点数：")));
    groupHLay4->addWidget(_adjustPoints4);
    groupHLay4->addWidget(_adjustPointsSetButton4);
    groupVLay4->addLayout(groupHLay4);
    _parmsTabContent4->setLayout(groupVLay4);

    //***【模拟量】调校参数 设定 tab子窗口
    _parmsTabContent5 = new QWidget;
    _adjustPoints5 = new QLineEdit;
    _adjustPointsSetButton5 = new QPushButton(tr("确定"));
    QVBoxLayout *groupVLay5 = new QVBoxLayout;
    QHBoxLayout *groupHLay5_1 = new QHBoxLayout;
    groupHLay5_1->addWidget(new QLabel(tr("模拟量通道号：")));
    groupHLay5_1->addWidget(_analogChannelNO);
    groupHLay5_1->addWidget(new QLabel(tr("模拟量类型：")));
    groupHLay5_1->addWidget(_analogChannelType);
    groupHLay5_1->addWidget(new QLabel(tr("校正点个数：")));
    groupHLay5_1->addWidget(_adjustPoints5);
    groupHLay5_1->addWidget(_adjustPointsSetButton5);
    groupHLay5_1->setSpacing(5);

    QHBoxLayout *groupHLay5_2 = new QHBoxLayout;
    groupHLay5_2->addWidget(_analogList);
    groupHLay5_2->addLayout(_analogWndsStackLayout);

    groupVLay5->addLayout(groupHLay5_1);
    groupVLay5->addLayout(groupHLay5_2);
    _parmsTabContent5->setLayout(groupVLay5);

    _parmsTabs = new QTabWidget;
    _parmsTabs->setFocusPolicy(Qt::NoFocus);
    _parmsTabs->addTab(_parmsTabContent1,tr("车速表调校参数"));
    _parmsTabs->addTab(_parmsTabContent2,tr("转速表调校参数"));
    _parmsTabs->addTab(_parmsTabContent3,tr("燃油表调校参数"));
    _parmsTabs->addTab(_parmsTabContent4,tr("水温表调校参数"));
    _parmsTabs->addTab(_parmsTabContent5,tr("模拟量调校参数"));

    //***Layout
    QVBoxLayout* top = new QVBoxLayout;
    QGroupBox *mainGroup = new QGroupBox(tr("调校参数"));
    //mainGroup->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    QVBoxLayout *mainGroupVlayout = new QVBoxLayout(mainGroup);
    mainGroupVlayout->addWidget(new QLabel,0,Qt::AlignHCenter);
    mainGroupVlayout->addWidget(_parmsTabs);

    QLabel* title = new QLabel(tr("调校参数配置"));
    title->setStyleSheet("background:#EFF7FD;color:#1E50A2;font:bold 16px;min-width:300px;max-height:30px;");
    title->setAlignment(Qt::AlignCenter);
    QHBoxLayout* bottom = new QHBoxLayout;
    bottom->addWidget(_readButton,0,Qt::AlignHCenter);
    bottom->addWidget(_saveButton,0,Qt::AlignHCenter);

    top->addWidget(title,1,Qt::AlignHCenter);
    top->addItem(new QSpacerItem(1,20));
    top->addWidget(mainGroup,1,Qt::AlignHCenter);
    top->addItem(new QSpacerItem(1,20));
    top->addLayout(bottom,1);
    top->setAlignment(bottom,Qt::AlignHCenter);
    setLayout(top);
    //***signal
    connect(_readButton,SIGNAL(clicked()),this,SLOT(on__readButton_clicked()));
    connect(_saveButton,SIGNAL(clicked()),this,SLOT(on__saveButton_clicked()));
    connect(_adjustPointsSetButton1,SIGNAL(clicked()),this,SLOT(showAdjustSettingControls()));
    connect(_adjustPointsSetButton2,SIGNAL(clicked()),this,SLOT(showAdjustSettingControls()));
    connect(_adjustPointsSetButton3,SIGNAL(clicked()),this,SLOT(showAdjustSettingControls()));
    connect(_adjustPointsSetButton4,SIGNAL(clicked()),this,SLOT(showAdjustSettingControls()));
    connect(_adjustPointsSetButton5,SIGNAL(clicked()),this,SLOT(showAdjustSettingControls1()));
    connect(_analogChannelNO,SIGNAL(valueChanged(int)),this,SLOT(analogChannelChanged(int)));
    connect(_analogList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(analogListClicked(QListWidgetItem*)));
    connect(_analogList,SIGNAL(itemDoubleClicked (QListWidgetItem*)),this,SLOT(analogListDoubleClicked(QListWidgetItem*)));
}

CConfigAdjustParmsWindow::~CConfigAdjustParmsWindow()
{

}

void CConfigAdjustParmsWindow::analogChannelChanged(int value)
{
    if(value >=0 && value<=5)
    {
        _analogChannelType->setCurrentIndex(_analogTypeNameMap["电流"]);
    }
    else if(value >=6 && value<=8)
    {
        _analogChannelType->setCurrentIndex(_analogTypeNameMap["电阻"]);
    }
    else if(value >=9 && value<=10)
    {
        _analogChannelType->setCurrentIndex(_analogTypeNameMap["电压"]);
    }
    else if(value >=11 && value<=13)
    {
        _analogChannelType->setCurrentIndex(_analogTypeNameMap["功率"]);
    }
}

void CConfigAdjustParmsWindow::analogListClicked(QListWidgetItem* item)
{
    int stackedIndex = item->text().right(2).toInt();
    //***因为设置了自动排序，而对应的stacklayout中的窗体却不被排序，所以要遍历
    for(int index=0; index != _analogWndsStackLayout->count(); ++index)
    {
        if(_analogWndsStackLayout->widget(index)->property("index").toInt() == stackedIndex)
        {
            _analogWndsStackLayout->setCurrentIndex(index);
            break;
        }
    }
}

void CConfigAdjustParmsWindow::analogListDoubleClicked(QListWidgetItem* item)
{
    int stackedIndex = item->text().right(2).toInt();

    int iRet = QMessageBox::question(this, tr("提示"),
                                     tr("是否要删除【模拟通道")+QString::number(stackedIndex)+"】的设置？",
                                     QMessageBox::Yes,
                                     QMessageBox::No);
    if(iRet == QMessageBox::Yes)
    {
        for(int j=0; j != _analogWndsStackLayout->count(); ++j)
        {
            if(_analogWndsStackLayout->widget(j)->property("index").toInt() == stackedIndex)
            {
                QWidget *wNeedRemove = _analogWndsStackLayout->widget(j);
                _analogWndsStackLayout->removeWidget(wNeedRemove);
                delete wNeedRemove;
                wNeedRemove = NULL;
                break;
            }
        }

        _analogList->takeItem(_analogList->row(item));
        delete item;
    }
}

void CConfigAdjustParmsWindow::showAdjustSettingControls()
{
    int tabIndex = _parmsTabs->currentIndex();
    QWidget *currentWidget = (qobject_cast<QWidget*>(sender()))->parentWidget();
    QVBoxLayout *currentVLay = qobject_cast<QVBoxLayout*>(currentWidget->layout());
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(currentVLay->itemAt(0)->layout()->itemAt(1)->widget());
    QString strNumberOfPointsInput = lineEdit->text();

    int numberOfPointsInput = strNumberOfPointsInput.toInt();
    if(numberOfPointsInput <=0 )
    {
        QMessageBox::information(this, tr("提示"), tr("调校点数设置有误！"),QMessageBox::Ok);
        lineEdit->setFocus();
        return;
    }
    else if(numberOfPointsInput > 8)
    {
        QMessageBox::information(this, tr("提示"), tr("调校点数最多为8个！"),QMessageBox::Ok);
        lineEdit->setFocus();
        return;
    }
    //***如果已经动态生成了 用户输入 数目个数的 调校点数，则移除
    if(currentVLay->layout()->count() > 1)
    {
        QLayoutItem * item;
        QLayout * sublayout;
        QWidget * widget;
        while ((item = currentVLay->itemAt(1)->layout()->takeAt(0))) {
            if ((sublayout = item->layout()) != 0) {
                //do the same for sublayout
            }
            else if ((widget = item->widget()) != 0) {
                widget->hide();
                delete widget;
            }
            else {
                delete item;
            }
        }
        delete currentVLay->itemAt(1)->layout();
    }
    //***根据 tab页 当前的索引 进行动态添加控件
    if(tabIndex == 0)
    {
        _adjustValue1.clear();
        _adjustAmount1.clear();
        QGridLayout *newGrid = new QGridLayout;
        newGrid->addWidget(new QLabel(tr("序号:")),0,0);
        newGrid->addWidget(new QLabel(tr("车速(kM/h):")),1,0);
        newGrid->addWidget(new QLabel(tr("预制微步数:")),2,0);
        for(int i=0; i< numberOfPointsInput; i++)
        {
            QLabel* label = new QLabel(QString::number(i+1));
            label->setStyleSheet("max-width:30px;max-height:24px;background:slateblue;");
            label->setAlignment(Qt::AlignCenter);
            QSpinBox* leTemp1 = new QSpinBox;
            leTemp1->setMaximum(999999999);
            leTemp1->setStyleSheet("max-width:60px;");
            _adjustValue1.push_back(leTemp1);
            QSpinBox* leTemp2 = new QSpinBox;
            leTemp2->setMaximum(999999999);
            leTemp2->setStyleSheet("max-width:60px;");
            _adjustAmount1.push_back(leTemp2);
            newGrid->addWidget(label,0,i+1);
            newGrid->addWidget(leTemp1,1,i+1);
            newGrid->addWidget(leTemp2,2,i+1);
        }
        currentVLay->addLayout(newGrid);

    }
    else if(tabIndex == 1)
    {
        _adjustValue2.clear();
        _adjustAmount2.clear();
        QGridLayout *newGrid = new QGridLayout;
        newGrid->addWidget(new QLabel(tr("序号:")),0,0);
        newGrid->addWidget(new QLabel(tr("转速(转/分钟):")),1,0);
        newGrid->addWidget(new QLabel(tr("预制微步数:")),2,0);
        for(int i=0; i< numberOfPointsInput; i++)
        {
            QLabel* label = new QLabel(QString::number(i+1));
            label->setStyleSheet("max-width:30px;max-height:24px;background:slateblue;");
            label->setAlignment(Qt::AlignCenter);
            QSpinBox* leTemp1 = new QSpinBox;
            leTemp1->setMaximum(999999999);
            leTemp1->setStyleSheet("max-width:60px;");
            _adjustValue2.push_back(leTemp1);
            QSpinBox* leTemp2 = new QSpinBox;
            leTemp2->setMaximum(999999999);
            leTemp2->setStyleSheet("max-width:60px;");
            _adjustAmount2.push_back(leTemp2);
            newGrid->addWidget(label,0,i+1);
            newGrid->addWidget(leTemp1,1,i+1);
            newGrid->addWidget(leTemp2,2,i+1);
        }
        currentVLay->addLayout(newGrid);
    }
    else if(tabIndex == 2)
    {
        _adjustValue3.clear();
        _adjustAmount3.clear();
        QGridLayout *newGrid = new QGridLayout;
        newGrid->addWidget(new QLabel(tr("序号:")),0,0);
        newGrid->addWidget(new QLabel(tr("燃油百分值(%):")),1,0);
        newGrid->addWidget(new QLabel(tr("预制微步数:")),2,0);
        for(int i=0; i< numberOfPointsInput; i++)
        {
            QLabel* label = new QLabel(QString::number(i+1));
            label->setStyleSheet("max-width:30px;max-height:24px;background:slateblue;");
            label->setAlignment(Qt::AlignCenter);
            QSpinBox* leTemp1 = new QSpinBox;
            leTemp1->setMaximum(999999999);
            leTemp1->setStyleSheet("max-width:60px;");
            _adjustValue3.push_back(leTemp1);
            QSpinBox* leTemp2 = new QSpinBox;
            leTemp2->setMaximum(999999999);
            leTemp2->setStyleSheet("max-width:60px;");
            _adjustAmount3.push_back(leTemp2);
            newGrid->addWidget(label,0,i+1);
            newGrid->addWidget(leTemp1,1,i+1);
            newGrid->addWidget(leTemp2,2,i+1);
        }
        currentVLay->addLayout(newGrid);
    }
    else if(tabIndex == 3)
    {
        _adjustValue4.clear();
        _adjustAmount4.clear();
        QGridLayout *newGrid = new QGridLayout;
        newGrid->addWidget(new QLabel(tr("序号:")),0,0);
        newGrid->addWidget(new QLabel(tr("水温(℃):")),1,0);
        newGrid->addWidget(new QLabel(tr("预制微步数:")),2,0);
        for(int i=0; i< numberOfPointsInput; i++)
        {
            QLabel* label = new QLabel(QString::number(i+1));
            label->setStyleSheet("max-width:30px;max-height:24px;background:slateblue;");
            label->setAlignment(Qt::AlignCenter);
            QSpinBox* leTemp1 = new QSpinBox;
            leTemp1->setMaximum(999999999);
            leTemp1->setStyleSheet("max-width:60px;");
            _adjustValue4.push_back(leTemp1);
            QSpinBox* leTemp2 = new QSpinBox;
            leTemp2->setMaximum(999999999);
            leTemp2->setStyleSheet("max-width:60px;");
            _adjustAmount4.push_back(leTemp2);
            newGrid->addWidget(label,0,i+1);
            newGrid->addWidget(leTemp1,1,i+1);
            newGrid->addWidget(leTemp2,2,i+1);
        }
        currentVLay->addLayout(newGrid);
    }
    currentWidget->setProperty("hasAdjust",true);
}
/*显示模拟量 调校配置界面*/
void CConfigAdjustParmsWindow::showAdjustSettingControls1()
{
    int numberOfPointsInput = _adjustPoints5->text().toInt();
    if(numberOfPointsInput <=0 )
    {
        QMessageBox::information(this, tr("提示"), tr("调校点数设置有误！"),QMessageBox::Ok);
        _adjustPoints5->setFocus();
        return;
    }
    else if(numberOfPointsInput > 8)
    {
        QMessageBox::information(this, tr("提示"), tr("调校点数最多为8个！"),QMessageBox::Ok);
        _adjustPoints5->setFocus();
        return;
    }
    bool bAddItem = false;
    //***获取用户选择的 通道号
    int analogNo = _analogChannelNO->value();
    //***检查是否已经进行了该通道的设置
    for(int i=0; i != _analogList->count(); ++i)
    {
        if(_analogList->item(i)->text().right(2).toInt() == analogNo)
        {
            int iRet = QMessageBox::question(this, tr("提示"),
                                             tr("已经添加了【模拟通道")+QString::number(analogNo)+"】的设置，是否要重新设置？",
                                             QMessageBox::Yes,
                                             QMessageBox::No);
            if(iRet == QMessageBox::Yes)
            {
                _analogList->setCurrentRow(i);
                for(int j=0; j != _analogWndsStackLayout->count(); ++j)
                {
                    if(_analogWndsStackLayout->widget(j)->property("index").toInt() == analogNo)
                    {
                        QWidget *wNeedRemove = _analogWndsStackLayout->widget(j);
                        _analogWndsStackLayout->removeWidget(wNeedRemove);
                        delete wNeedRemove;
                        break;
                    }
                }
            }
            else if(iRet == QMessageBox::No)
            {
                return;
            }
            bAddItem = true;
        }
    }

    QWidget *newWidget = new QWidget;
    QVBoxLayout *newMainVLayout = new QVBoxLayout;
    newMainVLayout->addWidget(new QLabel(tr("正在设置的通道：")+
                                         "<span style='color:darkgoldenrod;font:bold 15px;'>模拟通道"+QString::number(analogNo)+"</span>"));
    newMainVLayout->addWidget(new QLabel(tr("当前通道类型：")+
                                         "<span style='color:darkgoldenrod;font:bold 15px;'>"+_analogChannelType->currentText()+"</span>"));
    QLabel* tipText = new QLabel;
    tipText->setTextFormat(Qt::RichText);
    tipText->setText(
                tr("<ul><li style='color:orangered;margin:0;padding:0;list-style:circle;font:bold 15px;'>点击上面的<span style='color:darkgoldenrod;'>【确定】</span>按钮进行重新设置该通道</li>")+
                tr("<li style='color:orangered;margin:0;padding:0;list-style:circle;padding-left:40px;font:bold 15px;'><span style='color:darkgoldenrod;'>【双击】</span>左边列表可以删除该通道设置</li></ul>")
                );
    newMainVLayout->addWidget(tipText);
    QGridLayout *newGrid = new QGridLayout;
    newGrid->addWidget(new QLabel(tr("序号:")),0,0);
    newGrid->addWidget(new QLabel(tr("实际值:")),1,0);
    for(int i=0; i< numberOfPointsInput; i++)
    {
        QLabel* label = new QLabel(QString::number(i+1));
        label->setStyleSheet("max-width:30px;max-height:24px;background:steelblue;");
        label->setAlignment(Qt::AlignCenter);
        QSpinBox* leTemp1 = new QSpinBox;
        leTemp1->setMaximum(999999999);
        leTemp1->setStyleSheet("max-width:60px;");
        newGrid->addWidget(label,0,i+1);
        newGrid->addWidget(leTemp1,1,i+1);
    }
    newMainVLayout->setSizeConstraint(QLayout::SetFixedSize);
    newMainVLayout->addLayout(newGrid);
    newWidget->setLayout(newMainVLayout);
    //***添加项目 到 List列表控件
    if(!bAddItem)
    {
        QListWidgetItem *tempListItem = new QListWidgetItem;
        tempListItem->setData(Qt::UserRole,_analogChannelType->currentIndex());//保存通道类型
        tempListItem->setText(tr("模拟量通道 ") + QString().sprintf("%02d",analogNo));
        _analogList->addItem(tempListItem);
        _analogList->setCurrentItem (tempListItem);
    }
    //***使stack关联
    newWidget->setProperty("index",analogNo);
    _analogWndsStackLayout->addWidget(newWidget);
    _analogWndsStackLayout->setCurrentWidget(newWidget);
    _parmsTabContent5->setProperty("hasAdjust",true);
}

void CConfigAdjustParmsWindow::on__saveButton_clicked()
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
        QSettings *sIniSetting = new QSettings(filename,QSettings::IniFormat);
        sIniSetting->setIniCodec("UTF-8");

        if(_parmsTabContent1->property("hasAdjust").toBool())
        {
            QString value,step;
            for(int i=0; i < _adjustValue1.size();++i)
            {
                value += QString::number(_adjustValue1.at(i)->value()) + "\040";
            }
            value = value.left(value.length()-1);
            for(int i=0; i < _adjustAmount1.size();++i)
            {
                step += QString::number(_adjustAmount1.at(i)->value()) + "\040";
            }
            step = step.left(step.length()-1);
            sIniSetting->setValue("value/value_speed",value);
            sIniSetting->setValue("step/step_speed",step);
        }
        if(_parmsTabContent2->property("hasAdjust").toBool())
        {
            QString value,step;
            for(int i=0; i < _adjustValue2.size();++i)
            {
                value += QString::number(_adjustValue2.at(i)->value()) + "\040";
            }
            value = value.left(value.length()-1);
            for(int i=0; i < _adjustAmount2.size();++i)
            {
                step += QString::number(_adjustAmount2.at(i)->value()) + "\040";
            }
            step = step.left(step.length()-1);
            sIniSetting->setValue("value/value_rspeed",value);
            sIniSetting->setValue("step/step_rspeed",step);
        }
        if(_parmsTabContent3->property("hasAdjust").toBool())
        {
            QString value,step;
            for(int i=0; i < _adjustValue3.size();++i)
            {
                value += QString::number(_adjustValue3.at(i)->value()) + "\040";
            }
            value = value.left(value.length()-1);
            for(int i=0; i < _adjustAmount3.size();++i)
            {
                step += QString::number(_adjustAmount3.at(i)->value()) + "\040";
            }
            step = step.left(step.length()-1);
            sIniSetting->setValue("value/value_oil",value);
            sIniSetting->setValue("step/step_oil",step);
        }
        if(_parmsTabContent4->property("hasAdjust").toBool())
        {
            QString value,step;
            for(int i=0; i < _adjustValue4.size();++i)
            {
                value += QString::number(_adjustValue4.at(i)->value()) + "\040";
            }
            value = value.left(value.length()-1);
            for(int i=0; i < _adjustAmount4.size();++i)
            {
                step += QString::number(_adjustAmount4.at(i)->value()) + "\040";
            }
            step = step.left(step.length()-1);
            sIniSetting->setValue("value/value_wtemp",value);
            sIniSetting->setValue("step/step_wtemp",step);
        }
        if(_parmsTabContent5->property("hasAdjust").toBool())
        {
            QString anatype;
            for(int i=0; i!= _analogList->count(); ++i)
            {
                anatype += QString::number(_analogList->item(i)->data(Qt::UserRole).toInt()) + "\040";
            }
            anatype = anatype.left(anatype.length()-1);
            sIniSetting->setValue("anatype/anatype",anatype);

            for(int i=0; i!= _analogWndsStackLayout->count(); ++i)
            {
                QWidget* widget = _analogWndsStackLayout->widget(i);
                QGridLayout* grid = qobject_cast<QGridLayout*>(widget->layout()->itemAt(3)->layout());
                QString value;
                for(int j=1; j !=  grid->columnCount();++j)
                {
                    QSpinBox* spin = qobject_cast<QSpinBox*>(grid->itemAtPosition(1,j)->widget());
                    value += QString::number(spin->value()) + "\040";
                }
                value = value.left(value.length()-1);
                sIniSetting->setValue("value/value_ana_"+
                                      QString().sprintf("%02d",widget->property("index").toInt()),value);
            }

        }
        sIniSetting->sync();
        delete sIniSetting;
        QMessageBox::information(this, tr("提示"), tr("生成配置文件成功！"),QMessageBox::Ok);
    }
}

/*导入原始配置文件*/
void CConfigAdjustParmsWindow::on__readButton_clicked()
{
    bool hasAdjust = false;
    for(int i=0; i < _parmsTabs->count() ; ++i)
    {
        if(_parmsTabs->widget(i)->property("hasAdjust").toBool())
        {
            hasAdjust = true;
            break;
        }
    }
    if(hasAdjust)
    {
        int iRet = QMessageBox::question(this, tr("提示"),
                                         tr("当前已经有调校设置，是否重新设置调校设置界面？"),QMessageBox::Yes,QMessageBox::No);
        if(iRet != QMessageBox::Yes)
        {
            return;
        }
    }
    QString filename;
    QFileDialog dlg(this,"读取配置",QDir::currentPath(),"配置文件 (*.ini)");
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);

    if(dlg.exec() == QFileDialog::FileName)
    {
        filename = dlg.selectedFiles().first();
        QSettings *sIniSetting = new QSettings(filename, QSettings::IniFormat);
        sIniSetting->setIniCodec("UTF-8");
        //***定义临时变量
        QStringList valueList1,valueList2;
        int vi = 0;
        int vsize1,vsize2 = 0;
        QLayoutItem * item;
        QLayout * sublayout;
        QWidget * widget;
        QListWidgetItem* listwItem;
        //***导入车速配置
        valueList1 = sIniSetting->value("value/value_speed").toString().split(" ",QString::SkipEmptyParts);
        valueList2 = sIniSetting->value("step/step_speed").toString().split(" ",QString::SkipEmptyParts);
        vsize1  = valueList1.size();
        vsize2  = valueList2.size();
        if(vsize1 > 0 && vsize1 == vsize2)
        {
            _adjustPoints1->setText(QString::number(vsize1));
            QVBoxLayout *vlayout = qobject_cast<QVBoxLayout*>(_parmsTabContent1->layout());
            if(vlayout->layout()->count() > 1)
            {
                while ((item = vlayout->itemAt(1)->layout()->takeAt(0))) {
                    if ((sublayout = item->layout()) != 0) {
                        //do the same for sublayout
                    }
                    else if ((widget = item->widget()) != 0) {
                        widget->hide();
                        delete widget;
                    }
                    else {
                        delete item;
                    }
                }
                delete vlayout->itemAt(1)->layout();
            }

            QGridLayout *newGrid = new QGridLayout;
            newGrid->addWidget(new QLabel(tr("车速(KM/H):")),0,0);
            newGrid->addWidget(new QLabel(tr("预制微步数:")),1,0);
            _adjustValue1.clear();
            _adjustAmount1.clear();
            for(vi=0; vi<vsize1; vi++)
            {
                QSpinBox* leTemp1 = new QSpinBox;
                leTemp1->setMaximum(999999999);
                leTemp1->setStyleSheet("max-width:60px;");
                leTemp1->setValue(valueList1.at(vi).toInt());
                _adjustValue1.push_back(leTemp1);
                newGrid->addWidget(leTemp1,0,vi+1);

                QSpinBox* leTemp2 = new QSpinBox;
                leTemp2->setMaximum(999999999);
                leTemp2->setStyleSheet("max-width:60px;");
                leTemp2->setValue(valueList2.at(vi).toInt());
                _adjustAmount1.push_back(leTemp2);
                newGrid->addWidget(leTemp2,1,vi+1);
            }
            vlayout->addLayout(newGrid);
            _parmsTabContent1->setProperty("hasAdjust",true);
        }
        //***导入转速配置
        valueList1 = sIniSetting->value("value/value_rspeed").toString().split(" ",QString::SkipEmptyParts);
        valueList2 = sIniSetting->value("step/step_rspeed").toString().split(" ",QString::SkipEmptyParts);
        vsize1  = valueList1.size();
        vsize2  = valueList2.size();

        if(vsize1 > 0 && vsize1 == vsize2)
        {
            _adjustPoints2->setText(QString::number(vsize1));
            QVBoxLayout *vlayout = qobject_cast<QVBoxLayout*>(_parmsTabContent2->layout());
            if(vlayout->layout()->count() > 1)
            {
                while ((item = vlayout->itemAt(1)->layout()->takeAt(0))) {
                    if ((sublayout = item->layout()) != 0) {
                        //do the same for sublayout
                    }
                    else if ((widget = item->widget()) != 0) {
                        widget->hide();
                        delete widget;
                    }
                    else {
                        delete item;
                    }
                }
                delete vlayout->itemAt(1)->layout();
            }
            _adjustValue2.clear();
            _adjustAmount2.clear();
            QGridLayout *newGrid = new QGridLayout;
            newGrid->addWidget(new QLabel(tr("转速(转/分钟):")),0,0);
            newGrid->addWidget(new QLabel(tr("预制微步数:")),1,0);

            for(vi=0; vi<vsize1; vi++)
            {
                QSpinBox* leTemp1 = new QSpinBox;
                leTemp1->setMaximum(999999999);
                leTemp1->setStyleSheet("max-width:60px;");
                leTemp1->setValue(valueList1.at(vi).toInt());
                _adjustValue2.push_back(leTemp1);
                newGrid->addWidget(leTemp1,0,vi+1);

                QSpinBox* leTemp2 = new QSpinBox;
                leTemp2->setMaximum(999999999);
                leTemp2->setStyleSheet("max-width:60px;");
                leTemp2->setValue(valueList2.at(vi).toInt());
                _adjustAmount2.push_back(leTemp2);
                newGrid->addWidget(leTemp2,1,vi+1);
            }
            vlayout->addLayout(newGrid);
            _parmsTabContent2->setProperty("hasAdjust",true);
        }
        //***导入燃油配置
        valueList1 = sIniSetting->value("value/value_oil").toString().split(" ",QString::SkipEmptyParts);
        valueList2 = sIniSetting->value("step/step_oil").toString().split(" ",QString::SkipEmptyParts);
        vsize1  = valueList1.size();
        vsize2  = valueList2.size();

        if(vsize1 > 0 && vsize1 == vsize2)
        {
            _adjustPoints3->setText(QString::number(vsize1));
            QVBoxLayout *vlayout = qobject_cast<QVBoxLayout*>(_parmsTabContent3->layout());
            if(vlayout->layout()->count() > 1)
            {
                while ((item = vlayout->itemAt(1)->layout()->takeAt(0))) {
                    if ((sublayout = item->layout()) != 0) {
                        //do the same for sublayout
                    }
                    else if ((widget = item->widget()) != 0) {
                        widget->hide();
                        delete widget;
                    }
                    else {
                        delete item;
                    }
                }
                delete vlayout->itemAt(1)->layout();
            }
            _adjustValue3.clear();
            _adjustAmount3.clear();
            QGridLayout *newGrid = new QGridLayout;
            newGrid->addWidget(new QLabel(tr("燃油(%):")),0,0);
            newGrid->addWidget(new QLabel(tr("预制微步数:")),1,0);

            for(vi=0; vi<vsize1; vi++)
            {
                QSpinBox* leTemp1 = new QSpinBox;
                leTemp1->setMaximum(999999999);
                leTemp1->setStyleSheet("max-width:60px;");
                leTemp1->setValue(valueList1.at(vi).toInt());
                _adjustValue3.push_back(leTemp1);
                newGrid->addWidget(leTemp1,0,vi+1);

                QSpinBox* leTemp2 = new QSpinBox;
                leTemp2->setMaximum(999999999);
                leTemp2->setStyleSheet("max-width:60px;");
                leTemp2->setValue(valueList2.at(vi).toInt());
                _adjustAmount3.push_back(leTemp2);
                newGrid->addWidget(leTemp2,1,vi+1);
            }
            vlayout->addLayout(newGrid);
            _parmsTabContent3->setProperty("hasAdjust",true);
        }
        //***导入水温配置
        valueList1 = sIniSetting->value("value/value_wtemp").toString().split(" ",QString::SkipEmptyParts);
        valueList2 = sIniSetting->value("step/step_wtemp").toString().split(" ",QString::SkipEmptyParts);
        vsize1  = valueList1.size();
        vsize2  = valueList2.size();

        if(vsize1 > 0 && vsize1 == vsize2)
        {
            _adjustPoints4->setText(QString::number(vsize1));
            QVBoxLayout *vlayout = qobject_cast<QVBoxLayout*>(_parmsTabContent4->layout());
            if(vlayout->layout()->count() > 1)
            {
                while ((item = vlayout->itemAt(1)->layout()->takeAt(0))) {
                    if ((sublayout = item->layout()) != 0) {
                        //do the same for sublayout
                    }
                    else if ((widget = item->widget()) != 0) {
                        widget->hide();
                        delete widget;
                    }
                    else {
                        delete item;
                    }
                }
                delete vlayout->itemAt(1)->layout();
            }
            _adjustValue4.clear();
            _adjustAmount4.clear();
            QGridLayout *newGrid = new QGridLayout;
            newGrid->addWidget(new QLabel(tr("水温(℃):")),0,0);
            newGrid->addWidget(new QLabel(tr("预制微步数:")),1,0);

            for(vi=0; vi<vsize1; vi++)
            {
                QSpinBox* leTemp1 = new QSpinBox;
                leTemp1->setMaximum(999999999);
                leTemp1->setStyleSheet("max-width:60px;");
                leTemp1->setValue(valueList1.at(vi).toInt());
                _adjustValue4.push_back(leTemp1);
                newGrid->addWidget(leTemp1,0,vi+1);

                QSpinBox* leTemp2 = new QSpinBox;
                leTemp2->setMaximum(999999999);
                leTemp2->setStyleSheet("max-width:60px;");
                leTemp2->setValue(valueList2.at(vi).toInt());
                _adjustAmount4.push_back(leTemp2);
                newGrid->addWidget(leTemp2,1,vi+1);
            }
            vlayout->addLayout(newGrid);
            _parmsTabContent4->setProperty("hasAdjust",true);
        }
        //***导入模拟量调校配置
        while(item = _analogWndsStackLayout->layout()->takeAt(0))
        {
            if((widget = item->widget()) != 0)
            {
                widget->hide();
                delete widget;
            }
            else
            {
                delete item;
            }
        }
        while((listwItem = _analogList->takeItem(0)) != 0)
        {
            delete listwItem;
        }

        valueList1 = sIniSetting->value("anatype/anatype").toString().split(" ",QString::SkipEmptyParts);
        sIniSetting->beginGroup("value");
        valueList2 = sIniSetting->childKeys();
        QStringList slistTemp;
        int analogNo;
        if(vsize1 > 0 && vsize1 == vsize2)
        {
            foreach(QString key,valueList2)
            {
                if(key.contains("value_ana_"))
                {
                    slistTemp = sIniSetting->value(key).toString().split(" ",QString::SkipEmptyParts);
                    analogNo = key.section("_",2,2).toInt();
                    QWidget *newWidget = new QWidget;
                    QVBoxLayout *newMainVLayout = new QVBoxLayout;
                    newMainVLayout->addWidget(new QLabel(tr("正在设置的通道：")+
                                                         "<span style='color:darkgoldenrod;font:bold 15px;'>模拟通道"+QString::number(analogNo)+"</span>"));
                    newMainVLayout->addWidget(new QLabel(tr("当前通道类型：")+
                                                         "<span style='color:darkgoldenrod;font:bold 15px;'>"+_analogTypeNameMap.key(valueList1.at(analogNo).toInt())+"</span>"));
                    QLabel* tipText = new QLabel;
                    tipText->setTextFormat(Qt::RichText);
                    tipText->setText(
                                tr("<ul><li style='color:orangered;margin:0;padding:0;list-style:circle;font:bold 15px;'>点击上面的<span style='color:darkgoldenrod;'>【确定】</span>按钮进行重新设置该通道</li>")+
                                tr("<li style='color:orangered;margin:0;padding:0;list-style:circle;padding-left:40px;font:bold 15px;'><span style='color:darkgoldenrod;'>【双击】</span>左边列表可以删除该通道设置</li></ul>")
                                );
                    newMainVLayout->addWidget(tipText);
                    QGridLayout *newGrid = new QGridLayout;
                    newGrid->addWidget(new QLabel(tr("序号:")),0,0);
                    newGrid->addWidget(new QLabel(tr("实际值:")),1,0);
                    for(int i=0; i< slistTemp.size(); i++)
                    {
                        QLabel* label = new QLabel(QString::number(i+1));
                        label->setStyleSheet("max-width:30px;max-height:24px;background:steelblue;");
                        label->setAlignment(Qt::AlignCenter);
                        QSpinBox* leTemp1 = new QSpinBox;
                        leTemp1->setMaximum(999999999);
                        leTemp1->setValue(slistTemp.at(i).toInt());
                        leTemp1->setStyleSheet("max-width:60px;");
                        newGrid->addWidget(label,0,i+1);
                        newGrid->addWidget(leTemp1,1,i+1);
                    }
                    newMainVLayout->setSizeConstraint(QLayout::SetFixedSize);
                    newMainVLayout->addLayout(newGrid);
                    newWidget->setLayout(newMainVLayout);
                    //***添加项目 到 List列表控件

                    QListWidgetItem *tempListItem = new QListWidgetItem;
                    tempListItem->setData(Qt::UserRole,valueList1.at(analogNo).toInt());
                    //tempListItem->setText(tr("模拟量通道") + QString::number(analogNo));
                    tempListItem->setText(tr("模拟量通道 ") + QString().sprintf("%02d",analogNo));
                    _analogList->addItem(tempListItem);
                    _analogList->setCurrentItem (tempListItem);
                    //***使stack关联
                    newWidget->setProperty("index",analogNo);
                    _analogWndsStackLayout->addWidget(newWidget);
                    _parmsTabContent5->setProperty("hasAdjust",true);
                }
            }
            QMessageBox::information(this, tr("提示"), tr("导入调校参数配置成功！"),QMessageBox::Ok);
        }
        sIniSetting->endGroup();
        sIniSetting->sync();
        delete sIniSetting;
    }
}

void CConfigAdjustParmsWindow::keyReleaseEvent(QKeyEvent *event)
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
