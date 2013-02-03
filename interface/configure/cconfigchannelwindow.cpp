#include "cconfigchannelwindow.h"
#include "capp.h"

/**通道配置*/

CConfigChannelWindow::CConfigChannelWindow(QWidget *parent)
    : QWidget(parent)
{
    QStringList strList;
    strList << "IO 输出通道"<< "IO 输入通道"<< "频率通道"<< "电压通道"<< "电流通道"<< "电阻通道"<< "功率通道";
    QVBoxLayout* mainVLayout = new QVBoxLayout;
    _mainTab = new QTabWidget;
    _mainTab->setFocusPolicy(Qt::NoFocus);
    foreach(QString str, strList)
    {
        CConfigChannelTab<CConfigChannelTempleteWindow>* tab = new CConfigChannelTab<CConfigChannelTempleteWindow>(str,strList.indexOf(str));
        _mainTab->addTab(tab,str);
    }

    _readButton = new QPushButton(tr("打开应用参数配置 (Ctrl + O)"));
    _readButton->setStyleSheet("min-width:280px;min-height:48px;color:#333;font:bold 16px;");
    _saveButton = new QPushButton(tr("保存应用参数配置 (Ctrl + S)"));
    _saveButton->setStyleSheet("min-width:280px;min-height:48px;color:#333;font:bold 16px;");
    QHBoxLayout* bottom = new QHBoxLayout;
    bottom->addWidget(_readButton,0,Qt::AlignHCenter);
    bottom->addWidget(_saveButton,0,Qt::AlignHCenter);
    bottom->setSpacing(20);

    QLabel* title = new QLabel(tr("通道参数配置"));
    title->setStyleSheet("background:#EFF7FD;color:#1E50A2;font:bold 16px;min-width:300px;max-height:30px;");
    title->setAlignment(Qt::AlignCenter);
    mainVLayout->addWidget(title,0,Qt::AlignCenter);
    mainVLayout->addWidget(_mainTab);
    mainVLayout->addLayout(bottom);
    mainVLayout->setAlignment(bottom,Qt::AlignHCenter);
    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    setLayout(mainVLayout);

    connect(_readButton,SIGNAL(clicked()),SLOT(on__readButton_clicked()));
    connect(_saveButton,SIGNAL(clicked()),SLOT(on__saveButton_clicked()));
}

CConfigChannelWindow::~CConfigChannelWindow()
{

}

void CConfigChannelWindow::on__saveButton_clicked()
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

        CConfigChannelTempleteWindow* templetewnd;
        QStringList inisection;
        inisection << "map_ioin" << "map_ioout" << "map_freq" << "map_current" << "map_resistor" << "map_voltage" << "map_power";
        for(int tabIndex =0; tabIndex < _mainTab->count(); ++tabIndex)
        {
            templetewnd = qobject_cast<CConfigChannelTempleteWindow*>(_mainTab->widget(tabIndex));
            QWidget* widget;
            QLabel* labtemp;
            int row;
            QString channel,str;
            QStringList info;
            for(int i=0 ; i < templetewnd->_io_grid->count(); ++i)
            {
                if((widget = templetewnd->_io_grid->itemAt(i)->widget()) != 0)
                {
                    if(QString::compare(widget->metaObject()->className(),"QLabel") == 0)
                    {
                        labtemp = qobject_cast<QLabel*>(templetewnd->_io_grid->itemAt(i+1)->widget()->layout()->itemAt(0)->widget());

                        if(labtemp->text().length() < 1)
                        {
                            QMessageBox::information(this, tr("提示"), tr("还有未设置的通道！"),QMessageBox::Ok);
                            return;
                        }
                        info = labtemp->text().split("-");
                        row = static_cast<QString>(info.at(0)).toInt();
                        channel += QString::number(static_cast<QString>(info.at(1)).toInt() + (row-1)*20)+"\040";

                        if(tabIndex == 0)
                        {
                            QCheckBox* check = qobject_cast<QCheckBox*>(templetewnd->_io_grid->itemAt(i+1)->widget()->layout()->itemAt(1)->widget());
                            if(check->checkState() == Qt::Checked)
                                str += "1 ";
                            else
                                str += "0 ";
                        }
                    }
                }
            }
            if(tabIndex == 0)
            {
                str = str.left(str.length()-1);
                sIniSetting->setValue("iomap/map_ioin_def",str);
            }
            channel = channel.left(channel.length()-1);
            sIniSetting->setValue("iomap/"+inisection.at(tabIndex),channel);
        }

        sIniSetting->sync();
        delete sIniSetting;
        QMessageBox::information(this, tr("提示"), tr("保存成功！"),QMessageBox::Ok);
    }
}

void CConfigChannelWindow::on__readButton_clicked()
{
    QString filename;
    QFileDialog dlg(this,"读取配置",QDir::currentPath(),"配置文件 (*.ini)");
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);

    if(dlg.exec() == QFileDialog::FileName)
    {
        filename = dlg.selectedFiles().first();

        QSettings *sIniSetting = new QSettings(filename, QSettings::IniFormat);
        CConfigChannelTempleteWindow* templetewnd;
        QStringList info;
        QStringList inisection;
        inisection << "map_ioin" << "map_ioout" << "map_freq" << "map_current" << "map_resistor" << "map_voltage" << "map_power";

        for(int tabIndex =0; tabIndex < _mainTab->count(); ++tabIndex)
        {
            info.clear();
            QVariant eachSection = sIniSetting->value("iomap/" + inisection.at(tabIndex));
            if(eachSection.isValid() && eachSection.toString().length() > 0)
                info = eachSection.toString().split(" ",QString::SkipEmptyParts);
            if(info.size() == 0)
                continue;

            templetewnd = qobject_cast<CConfigChannelTempleteWindow*>(_mainTab->widget(tabIndex));

            //***询问用户是否重置，如果重置..
            if(templetewnd->_io_grid->count() > 0)
            {
                int iRet = QMessageBox::warning(this, tr("警告"),
                                                tr("您已经开始编辑了通道设置，确定要重置吗？"),
                                                QMessageBox::Yes,
                                                QMessageBox::No);
                if(iRet == QMessageBox::Yes)
                {
                    QGridLayout* grid;
                    QLayoutItem* item;
                    QWidget * widget;
                    //***移除左边的控件
                    while((item = templetewnd->_io_grid->takeAt(0)))
                    {
                        if ((widget = item->widget()) != 0)
                        {
                            widget->hide();
                            delete widget;
                        }
                        else
                        {
                            delete item;
                        }
                    }
                    //***重置右边的状态
                    for(int index=0; index != templetewnd->_stack->count(); ++index)
                    {
                        grid = qobject_cast<QGridLayout*>(templetewnd->_stack->widget(index)->layout());
                        for(int i=0 ; i < grid->count(); ++i)
                        {
                            if(widget = grid->itemAt(i)->widget())
                            {
                                if(grid->itemAt(i)->widget()->property("click").isValid())
                                {
                                    widget->setProperty("click",false);
                                    widget->setStyleSheet("border-radius:15px;background:white;border:1px solid #999;");
                                }
                            }
                        }
                    }
                    widget = templetewnd->_rightVLayout->itemAt(0)->widget();
                    widget->hide();
                    delete widget; //***唯有移除widget使id属性不合法
                    templetewnd->_rightVLayout->insertWidget(0,new QLabel(tr("正在设置：")+"<span style='color:green;font:bold 15px;'></span>"));
                }
                else if(iRet == QMessageBox::No)
                {
                    return;
                }
            }
            (qobject_cast<QLineEdit*>(templetewnd->_io_tophbox->itemAt(1)->widget()))->setText(QString::number(info.size()));
            //***添加通道控件 和 对应设置结果控件
            for(int i=0; i != info.size(); ++i)
            {
                QGroupBox* tempGroup = new QGroupBox;
                tempGroup->setProperty("type","special");
                QVBoxLayout* vbox = new QVBoxLayout(tempGroup);
                vbox->setMargin(0);
                vbox->setContentsMargins(0,0,0,0);
                QLabel *temp1 = new QLabel(QString::number(i+1));
                temp1->setStyleSheet("border-radius:15px;background:#CCCCCC;border:1px solid #666;");
                temp1->setAlignment(Qt::AlignCenter);
                temp1->setFixedSize(30,30);
                temp1->setProperty("circle",1);
                temp1->installEventFilter(templetewnd);

                QLabel *temp2 = new QLabel;
                temp2->setAlignment(Qt::AlignCenter);
                temp2->setFixedSize(30,30);

                int data = info.at(i).toInt();
                int row = data>20 ? 1 : 0;
                int channel = data>20 ? (data%20+20) : data;
                temp2->setText(QString::number(row+1)+"-"+QString::number(channel));
                temp2->setStyleSheet("border-radius:15px;border:2px dotted #66CCFF;background:#66FFCC");

                vbox->addWidget(temp2);
                if(tabIndex == 0)
                {
                    static QStringList ioin_def = sIniSetting->value("iomap/map_ioin_def").toString().split(" ",QString::SkipEmptyParts);
                    QCheckBox* check = new QCheckBox;
                    if(ioin_def.at(i).toInt() == 1)
                    {
                        check->setText(tr("高"));
                        check->setCheckState(Qt::Checked);
                        check->setStyleSheet("QCheckBox{background:red;}");
                    }
                    else
                    {
                        check->setText(tr("低"));
                        check->setStyleSheet("QCheckBox{background:green;}");
                    }
                    check->setFocusPolicy(Qt::NoFocus);
                    vbox->addWidget(check);
                    connect(check,SIGNAL(stateChanged(int)),templetewnd,SLOT(checkStateChanged(int)));
                }
                templetewnd->_io_grid->addWidget(temp1,i/20 == 0 ? 0 : 2,i/20 >=1 ? i%20 : i,1,1,Qt::AlignCenter);
                templetewnd->_io_grid->addWidget(tempGroup,i/20== 0 ? 1 : 3,i/20 >=1 ? i%20 : i,1,1,Qt::AlignCenter);
                //***更改右边状态
                for(int m=0; m != templetewnd->_stack->count(); ++m)
                {
                    if(m == row)
                    {
                        QGridLayout* grid = qobject_cast<QGridLayout*>(templetewnd->_stack->widget(m)->layout());
                        for(int n=0 ; n < grid->count(); ++n)
                        {
                            QLabel* lab = qobject_cast<QLabel*>(grid->itemAt(n)->widget());
                            if(qobject_cast<QLabel*>(grid->itemAt(n)->widget())->text().toInt() == channel)
                            {
                                lab->setProperty("id",i+1);
                                lab->setProperty("click",true);
                                lab->setStyleSheet("border-radius:15px;border:0;color:white;background:gray");
                            }
                        }
                    }
                }
            }
        }
        sIniSetting->sync();
        delete sIniSetting;
    }
}

void CConfigChannelWindow::keyReleaseEvent(QKeyEvent *event)
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
