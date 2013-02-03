#ifndef CONFIG_CHANNEL_TEMPLETE_H
#define CONFIG_CHANNEL_TEMPLETE_H

#include <QWidget>
#include <QtGui>


class CConfigChannelTempleteWindow : public QWidget
{
    Q_OBJECT
public:
    CConfigChannelTempleteWindow(const QString& title,int tabIndex,QWidget *parent = 0)
    {
        QHBoxLayout  *globalHLayout = new QHBoxLayout;
        QGroupBox* rightGroup = new QGroupBox(tr("通道选择"));
        QGroupBox* io_group = new QGroupBox(title);

        QVBoxLayout *io_vlayout = new QVBoxLayout(io_group);

        _io_tophbox = new QHBoxLayout;
        _io_tophbox->addWidget(new QLabel(tr("请输入通道数：")));
        _io_tophbox->addWidget(new QLineEdit);
        _io_tophbox->addWidget(new QPushButton(tr("确定")));

        QFrame *ioframe = new QFrame;
        _io_grid = new QGridLayout(ioframe);
        ioframe->setStyleSheet("border:2px ridge #9999CC;");

        io_vlayout->addLayout(_io_tophbox);
        io_vlayout->addWidget(ioframe);

        _rightVLayout = new QVBoxLayout(rightGroup);
        QHBoxLayout* rightHLayout = new QHBoxLayout;

        _board1 = new QWidget;
        _channel_grid1 = new QGridLayout(_board1);
        _channel_grid1->addWidget(new QLabel(tr("<span style='background:#EFF7FD;color:#1E50A2;font:bold 16px;'>输入板1</span>")),0,4,1,3);
        for(int i=0; i!=20; ++i)
        {
            QLabel *lab = new QLabel(QString::number(i+1));

            lab->setStyleSheet("border-radius:15px;background:white;border:1px solid #666;");
            lab->setAlignment(Qt::AlignCenter);
            lab->setFixedSize(30,30);
            lab->setProperty("circle",2);
            lab->setProperty("click",false);
            lab->installEventFilter(this);

            _channel_grid1->addWidget(lab,i/10+1,i/10 >=1 ? i%10 : i,1,1,Qt::AlignCenter);
        }

        _board2 = new QWidget;
        _channel_grid2 = new QGridLayout(_board2);
        _channel_grid2->addWidget(new QLabel(tr("<span style='background:#EFF7FD;color:#1E50A2;font:bold 16px;'>输入板2</span>")),0,4,1,3);
        for(int i=0; i!=20; ++i)
        {
            QLabel *lab = new QLabel(QString::number(i+1));

            lab->setStyleSheet("border-radius:15px;background:white;border:1px solid #666;");
            lab->setAlignment(Qt::AlignCenter);
            lab->setFixedSize(30,30);
            lab->setProperty("circle",2);
            lab->setProperty("click",false);
            lab->installEventFilter(this);

            _channel_grid2->addWidget(lab,i/10+1,i/10 >=1 ? i%10 : i,1,1,Qt::AlignCenter);
        }

        _list = new QListWidget;
        _list->addItems( QStringList() << tr("NO1. 输入板") << tr("NO2. 输出板") );
        _list->setFixedSize(100,80);
        _list->setCurrentRow(0);

        _stack = new QStackedLayout;
        rightHLayout->addWidget(_list,0,Qt::AlignTop);
        rightHLayout->addLayout(_stack);
        _rightVLayout->addWidget(new QLabel(tr("正在设置：")+
                                            "<span style='color:green;font:bold 15px;'></span>"));
        _rightVLayout->addLayout(rightHLayout);
        //***整合并渲染布局

        globalHLayout->addWidget(io_group);
        globalHLayout->addWidget(rightGroup);

        rightGroup->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
        this->setLayout(globalHLayout);

        //***other
        _io_tophbox->itemAt(2)->widget()->setObjectName("setiocount");
        _list->setObjectName("list");
        _tabIndex = tabIndex;
        _stack->addWidget(_board1);
        _stack->addWidget(_board2);
        _stack->setCurrentIndex(0);
        QMetaObject::connectSlotsByName(this);
    }
    ~CConfigChannelTempleteWindow()
    {

    }

public:
     QGridLayout *_io_grid;
     QHBoxLayout *_io_tophbox;
     QStackedLayout* _stack;
     QVBoxLayout* _rightVLayout;
private:
    int _tabIndex;
    QGridLayout *_channel_grid1;
    QGridLayout *_channel_grid2;
    QListWidget* _list;

    QWidget* _board1;
    QWidget* _board2;
public slots:

protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        QLabel* lab =  qobject_cast<QLabel*>(obj) ;
        //***圆圈QLabel
        if (event->type() == QEvent::MouseButtonPress)
        {
            if(obj->property("circle").toInt() == 1)
            {
                QLabel *tip = qobject_cast<QLabel*>(_rightVLayout->itemAt(0)->widget());
                tip->setText(tr("正在设置：")+
                             "<span style='background:#EFF7FD;color:blue;font:bold 14px;'>"+
                             qobject_cast<QGroupBox*>(lab->parentWidget()->parentWidget())->title()+
                             "</span>"+"<span style='color:red;font:bold 16px;'>"+lab->text()+"</span>");
                tip->setProperty("id",lab->text().toInt());
            }
            else if(lab->property("circle").toInt() == 2)
            {
                QVariant var = qobject_cast<QWidget*>(_rightVLayout->itemAt(0)->widget())->property("id");

                if(!var.isValid())
                {
                    return true;
                }
                bool checked = !lab->property("click").toBool();
                int id = var.toInt();
                int row =  _list->currentRow();
                //***判断是否已经设置了
                QGridLayout* grid  = 0;
                for(int index=0; index != _stack->count(); ++index)
                {
                    grid = qobject_cast<QGridLayout*>(_stack->widget(index)->layout());
                    for(int i=0 ; i < grid->count(); ++i)
                    {
                        if(grid->itemAt(i)->widget() == lab)
                        {
                            if(grid->itemAt(i)->widget()->property("click").toBool())
                            {

                                if((grid->itemAt(i)->widget()->property("id").isValid()) &&
                                        (grid->itemAt(i)->widget()->property("id").toInt() != id))
                                {
                                    QMessageBox::information(this, tr("提示"), tr("通道已被设置,请先移除后在更改！"),QMessageBox::Ok);

                                    return true;
                                }
                            }
                        }
                        else
                        {
                            if((grid->itemAt(i)->widget()->property("click").toBool())&&
                                    (grid->itemAt(i)->widget()->property("id").toInt() == id))
                            {
                                QMessageBox::information(this, tr("提示"), tr("一个通道只能匹配一个,请先移除后在更改！"),QMessageBox::Ok);
                                return true;
                            }
                        }
                    }
                }
                QWidget* widget;
                QLabel* labtemp;
                for(int i=0 ; i < _io_grid->count(); ++i)
                {
                    if((widget = _io_grid->itemAt(i)->widget()) != 0)
                    {
                        if(QString::compare(widget->metaObject()->className(),"QLabel") == 0)
                        {
                            labtemp = qobject_cast<QLabel*>(widget);
                            if(labtemp->text().compare(QString::number(id)) == 0)
                            {
                                labtemp = qobject_cast<QLabel*>(_io_grid->itemAt(i+1)->widget()->layout()->itemAt(0)->widget());

                                if(checked)
                                {
                                    labtemp->setStyleSheet("border-radius:15px;border:2px dotted #66CCFF;background:#66FFCC");
                                    labtemp->setText(QString::number(row+1)+"-"+lab->text());
                                }
                                else
                                {
                                    labtemp->setStyleSheet("border-radius:15px;background:white;border:1px solid #999;");
                                    labtemp->setText("");
                                }
                            }
                        }
                    }
                }
                if(checked)
                {
                    lab->setStyleSheet("border-radius:15px;border:0;color:white;background:gray");
                }
                else
                {
                    lab->setStyleSheet("border-radius:15px;border:1px solid #666;background:white");
                }
                lab->setProperty("id",id);
                lab->setProperty("click",checked);              
            }
            return true;
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {

            return true;
        }
        //***后面是鼠标效果处理
        else if(event->type() == QEvent::Enter)
        {
            if(!obj->property("click").isValid() || !obj->property("click").toBool())
            {
                lab->setStyleSheet("border-radius:15px;border:2px dotted #66CCFF;background:#66FFCC");
            }
            return true;
        }
        else if(event->type() == QEvent::Leave)
        {
            if(obj->property("circle").toInt() == 1)
            {
                lab->setStyleSheet("border-radius:15px;border:1px solid #666;background:#CCCCCC");
            }
            else if(obj->property("circle").toInt() == 2)
            {
                if(!obj->property("click").toBool())
                {
                    lab->setStyleSheet("border-radius:15px;border:1px solid #666;background:white");
                }
            }
            return true;
        }

        return QWidget::eventFilter(obj, event);
    }
private slots:
    void on_setiocount_clicked()
    {
        {
            int count = (qobject_cast<QLineEdit*>(_io_tophbox->itemAt(1)->widget())->text().toInt());
            if(count > 40)
            {
                QMessageBox::warning(this, tr("警告"), tr("通道数最大不超过40！"),QMessageBox::Ok);
                return;
            }
            //***询问用户是否重置，如果重置..
            if(_io_grid->count() > 0)
            {
                int iRet = QMessageBox::warning(this, tr("警告"),
                                                tr("更改通道数目将导致该通道的所有配置失效，确定要重置吗？"),
                                                QMessageBox::Yes,
                                                QMessageBox::No);
                if(iRet == QMessageBox::Yes)
                {
                    QGridLayout* grid;
                    QLayoutItem* item;
                    QWidget * widget;
                    //***移除左边的控件
                    while((item = _io_grid->takeAt(0)))
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
                    for(int index=0; index != _stack->count(); ++index)
                    {
                        grid = qobject_cast<QGridLayout*>(_stack->widget(index)->layout());
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
                    widget = _rightVLayout->itemAt(0)->widget();
                    widget->hide();
                    delete widget; //***唯有移除widget使id属性不合法
                    _rightVLayout->insertWidget(0,new QLabel(tr("正在设置：")+"<span style='color:green;font:bold 15px;'></span>"));
                }
                else if(iRet == QMessageBox::No)
                {
                    return;
                }
            }
            //***添加通道控件 和 对应设置结果控件
            for(int i=0; i != count; ++i)
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
                temp1->installEventFilter(this);

                QLabel *temp2 = new QLabel;
                temp2->setStyleSheet("border-radius:15px;background:white;border:1px solid #999;");
                temp2->setAlignment(Qt::AlignCenter);
                temp2->setFixedSize(30,30);

                vbox->addWidget(temp2);
                if(_tabIndex == 0)
                {
                    QCheckBox* check = new QCheckBox(tr("低"));
                    check->setStyleSheet("QCheckBox{background:green;}");
                    check->setFocusPolicy(Qt::NoFocus);
                    vbox->addWidget(check);
                    connect(check,SIGNAL(stateChanged(int)),this,SLOT(checkStateChanged(int)));
                }
                _io_grid->addWidget(temp1,i/20 == 0 ? 0 : 2,i/20 >=1 ? i%20 : i,1,1,Qt::AlignCenter);
                _io_grid->addWidget(tempGroup,i/20== 0 ? 1 : 3,i/20 >=1 ? i%20 : i,1,1,Qt::AlignCenter);
            }
        }
    }
    void on_list_currentRowChanged(int row)
    {
        _stack->setCurrentIndex(row);
    }
    void checkStateChanged(int state)
    {
        QCheckBox* check = qobject_cast<QCheckBox*>(sender());
        if(state == Qt::Checked)
        {
            check->setText(tr("高"));
            check->setStyleSheet("QCheckBox{background:red;}");
        }
        else
        {
            check->setText(tr("低"));
            check->setStyleSheet("QCheckBox{background:green;}");
        }
    }
};

template<typename BaseTabWnd>
class CConfigChannelTab : public BaseTabWnd
{
public:
    CConfigChannelTab(const QString& title,int tabIndex,QWidget *parent = 0)
        :BaseTabWnd(title,tabIndex,parent)
    {

    }
    ~CConfigChannelTab()
    {

    }
};
#endif
