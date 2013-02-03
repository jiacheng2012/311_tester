#include "capp.h"
#include "CAdjustTestWaterTemperatureWindow.h"

CAdjustTestWaterTemperatureWindow::CAdjustTestWaterTemperatureWindow(QWidget *parent) :
    QWidget(parent)
{
    //***UI CONPONENTS
    setProperty("testFlag",0);
    setProperty("zero",0);
    setProperty("id", 0); //当前调校是 第几项
    setProperty("read",-1);
    setProperty("type",-1);
    setProperty("number",0);
    setProperty("wait0",0);

    QVBoxLayout *_top = new QVBoxLayout();

    _statusLabel = new QLabel(tr("测试前请先把指针归零。快捷键为："));
    _statusLabel->setStyleSheet("font:bold 16px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");

    _currentItem = new QLabel;
    _currentItem->setPixmap(QPixmap(":/res/flag.png"));
    _currentItem->setFixedSize(40,30);
    _currentItem->setScaledContents(true);

    for(int i=0; i<_MAX_MADJUST_POINT_; i++)
    {
        _theoryValue[i] = new QLabel;//理论采样值
        _currentStep[i] = new QSpinBox;
        _sampleValue[i] = new QLabel;//仪表采样值

        _adjustButton[i]= new QPushButton(tr("测试"));//调校按钮

        _currentStep[i]->setMinimum(-5000);
        _currentStep[i]->setMaximum(5000);

        connect(_adjustButton[i],SIGNAL(clicked()),this,SLOT(adjustButtonClicked()));
        connect(_currentStep[i],SIGNAL(valueChanged(int)),this,SLOT(stepValueChanged(int)));
    }

    _currentAmount = new QSpinBox;//当前微步数
    _currentAmount->setMinimum(1);
    _currentAmount->setMaximum(500);
    _currentAmount->setValue(20);
    _zeroButton = new QPushButton(tr("指针归零(Z)"));
    _increaseButton = new QPushButton(tr("增加"));//增加微步数
    _decreaseButton = new QPushButton(tr("减少"));//减少微步数

    _savaAllAdjustButton = new QPushButton(tr("保存水温表调校参数\n(CTRL + S)"));//保存所有调校
    _readButton = new QPushButton(tr("读取水温表调校参数\n(R)"));

    _savaStatusLabel = new QLabel;//保存调校完成状态显示 label
    _savaStatusLabel->setStyleSheet("background:wheat;");
    _writeButton = new QPushButton(tr("参数固化写入仪表"));

    //***Layout
    QGroupBox *_centerGroupBox = new QGroupBox(tr("水温表调校"));
    _centerGrid = new QGridLayout(_centerGroupBox);
    _centerGrid->addWidget(_statusLabel,0,0,1,_MAX_MADJUST_POINT_+1);
    _centerGrid->addWidget(_zeroButton,1,0);
    _centerGrid->addWidget(new QLabel(tr("微步数增减幅度：")),2,0);
    _centerGrid->addWidget(_currentAmount,2,1,1,2);
    _centerGrid->addWidget(_increaseButton,2,3,1,1);
    _centerGrid->addWidget(_decreaseButton,2,4,1,1);
    _centerGrid->addWidget(new QLabel(tr("当前调校项：")),3,0);
    _centerGrid->addWidget(new QLabel(tr("理论值 (度)：")),4,0);
    _centerGrid->addWidget(new QLabel(tr("现在微步数：")),5,0);
    _centerGrid->addWidget(new QLabel(tr("仪表采样值：")),6,0);
    _centerGrid->addWidget(new QLabel(tr("调校操作：")),7,0);

    for(int j=0; j != _MAX_MADJUST_POINT_; ++j)
    {
        _centerGrid->addWidget(_theoryValue[j],4,j+1);//理论采样值
        _centerGrid->addWidget(_currentStep[j],5,j+1);//仪表板采样值
        _centerGrid->addWidget(_sampleValue[j],6,j+1);//预置微步数
        _centerGrid->addWidget(_adjustButton[j],7,j+1);//仪表板采样值
    }
    _centerGrid->addWidget(_currentItem,3,1);
    _centerGrid->addWidget(_savaStatusLabel,8,1,1,4);
    _centerGrid->addWidget(new QLabel(tr("存储操作：")),8,0);
    _centerGrid->addWidget(_readButton,8,5,1,2);
    _centerGrid->addWidget(_savaAllAdjustButton,8,7,1,2);
    _centerGrid->addWidget(_writeButton,9,5,1,4);

    _top->addWidget(_centerGroupBox);
    _top->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(_top);

    //***signals
    connect(_zeroButton,SIGNAL(clicked()),this,SLOT(zeroButtonClicked()));
    connect(_increaseButton,SIGNAL(clicked()),this,SLOT(increaseButtonClicked()));
    connect(_decreaseButton,SIGNAL(clicked()),this,SLOT(decreaseButtonClicked()));
    connect(_savaAllAdjustButton,SIGNAL(clicked()),this,SLOT(saveAllAdjustButtonClicked()));
    connect(_writeButton,SIGNAL(clicked()),this,SLOT(writeButtonClicked()));
    connect(_readButton,SIGNAL(clicked()),this,SLOT(readButtonClicked()));

    connect((CApp*)qApp,SIGNAL(sendBackAdjustTestData_watertemp(CDataFrame)),this,SLOT(sendBackData(CDataFrame)));
    //***
    cf = ((CApp*)qApp)->_tjob->_aconfig;

    vu16    value;
    v16     sample;

    for(int i=0;i<cf->value_meter["wtemp"].size();i++)
    {
        value[i] = cf->value_meter["wtemp"][i];
        sample[i] = cf->sample_meter["wtemp"][i];
    }

    updateInterface(cf->value_meter["wtemp"].size(),value,sample);
}

void CAdjustTestWaterTemperatureWindow::zeroButtonClicked()
{
    sendFrame(0x3,0x0,0x0);
    setProperty("zero",1);
    _zeroButton->setEnabled(false);
}

void CAdjustTestWaterTemperatureWindow::increaseButtonClicked()
{
    if(property("testFlag").toBool())
    {
        int newv = _currentStep[property("id").toInt()]->value() + _currentAmount->value();
        _currentStep[property("id").toInt()]->setValue(newv);
        sendFrame(0x3,0x1,newv);
    }
}

void CAdjustTestWaterTemperatureWindow::decreaseButtonClicked()
{
    if(property("testFlag").toBool())
    {
        int newv = _currentStep[property("id").toInt()]->value() - _currentAmount->value();
        _currentStep[property("id").toInt()]->setValue(newv);
        sendFrame(0x3,0x1,newv);
    }
}

void CAdjustTestWaterTemperatureWindow::adjustButtonClicked()
{
    QPushButton* sc=qobject_cast<QPushButton*>(sender());

    for(int i=0;i<property("number").toInt();i++)
    {
        if(sc ==_adjustButton[i])
        {
            setProperty("id",i);
            break;
        }
    }

    setProperty("testFlag",1);
    _currentStep[property("id").toInt()]->setFocus();
    sendFrame(0x3,0x1,_currentStep[property("id").toInt()]->value());
    _centerGrid->removeWidget(_currentItem);
    _centerGrid->addWidget(_currentItem,3,property("id").toInt()+1);
}

void CAdjustTestWaterTemperatureWindow::stepValueChanged(int v)
{
    QSpinBox* sc=qobject_cast<QSpinBox*>(sender());
    int id=-1;
    for(int i=0;i<property("number").toInt();i++)
    {
        if(sc ==_currentStep[i])
        {
            setProperty("id",i);
            break;
        }
    }

    setProperty("testFlag",1);
    sendFrame(0x3,0x1,_currentStep[property("id").toInt()]->value());
    _centerGrid->removeWidget(_currentItem);
    _centerGrid->addWidget(_currentItem,3,property("id").toInt()+1);
}

void CAdjustTestWaterTemperatureWindow::sendFrame(quint8 c,quint8 m,qint16 s)
{
    adjustCommandFrame_0x40 b={_METER_FRAME_COMMAND_ADJUST_,0x00};
    b.command = c;
    b.motor = m;
    QByteArray ba = arrangeByte(s,2);
    b.step[0] = ba[0];
    b.step[1] = ba[1];
    QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
    ((CApp*)qApp)->_tjob->sendTestData232(tmp);
}

void CAdjustTestWaterTemperatureWindow::sendBackData(CDataFrame a)
{
    if(property("testFlag").toBool()||property("read").toInt()!=-1 ||
            property("zero").toInt())
    {
        QByteArray d = a.getData();

        if(a._command == _METER_FRAME_INIT_PRESPONSE_ADJUST_)
        {
            if(property("read").toInt()==1)
            {
                adjustInitPResponseFrame_0x51 rt = frame51FromData(d);

                int num = rt.num_meter[2];

                adjustSaveParaFrame_0x42_0x45 b={_METER_FRAME_SAVE_PARA_WTEMP_ADJUST_,0x00};
                b.command = (0x01<<4);
                b.number = num;

                QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
                ((CApp*)qApp)->_tjob->sendTestData232(tmp);
            }
            else if(property("read").toInt()==0)
            {
                //***保存到结构
                adjustSaveParaFrame_0x42_0x45 b={_METER_FRAME_SAVE_PARA_WTEMP_ADJUST_,0x00};
                b.command = (0x02<<4);
                b.number = property("number").toInt();

                QByteArray tmp;
                for(int i=0;i<property("number").toInt();i++)
                {
                    cf->value_meter["wtemp"][i] = _theoryValue[i]->text().toInt();
                    cf->sample_meter["wtemp"][i] = _sampleValue[i]->text().toInt();
                }

                if(property("number").toInt()>0)
                {
                    tmp = arrangeByte(_sampleValue[0]->text().toInt(),2);
                    b.sample0[0] = tmp[0];
                    b.sample0[1] = tmp[1];
                    tmp = arrangeByte(_theoryValue[0]->text().toInt()+40,2);
                    b.value0[0] = tmp[0];
                    b.value0[1] = tmp[1];
                }
                if(property("number").toInt()>1)
                {
                    tmp = arrangeByte(_sampleValue[1]->text().toInt(),2);
                    b.sample1[0] = tmp[0];
                    b.sample1[1] = tmp[1];
                    tmp = arrangeByte(_theoryValue[1]->text().toInt()+40,2);
                    b.value1[0] = tmp[0];
                    b.value1[1] = tmp[1];
                }
                if(property("number").toInt()>2)
                {
                    tmp = arrangeByte(_sampleValue[2]->text().toInt(),2);
                    b.sample2[0] = tmp[0];
                    b.sample2[1] = tmp[1];
                    tmp = arrangeByte(_theoryValue[2]->text().toInt()+40,2);
                    b.value2[0] = tmp[0];
                    b.value2[1] = tmp[1];
                }
                if(property("number").toInt()>3)
                {
                    tmp = arrangeByte(_sampleValue[3]->text().toInt(),2);
                    b.sample3[0] = tmp[0];
                    b.sample3[1] = tmp[1];
                    tmp = arrangeByte(_theoryValue[3]->text().toInt()+40,2);
                    b.value3[0] = tmp[0];
                    b.value3[1] = tmp[1];
                }
                if(property("number").toInt()>4)
                {
                    tmp = arrangeByte(_sampleValue[4]->text().toInt(),2);
                    b.sample4[0] = tmp[0];
                    b.sample4[1] = tmp[1];
                    tmp = arrangeByte(_theoryValue[4]->text().toInt()+40,2);
                    b.value4[0] = tmp[0];
                    b.value4[1] = tmp[1];
                }
                if(property("number").toInt()>5)
                {
                    tmp = arrangeByte(_sampleValue[5]->text().toInt(),2);
                    b.sample5[0] = tmp[0];
                    b.sample5[1] = tmp[1];
                    tmp = arrangeByte(_theoryValue[5]->text().toInt()+40,2);
                    b.value5[0] = tmp[0];
                    b.value5[1] = tmp[1];
                }
                if(property("number").toInt()>6)
                {
                    tmp = arrangeByte(_sampleValue[6]->text().toInt(),2);
                    b.sample6[0] = tmp[0];
                    b.sample6[1] = tmp[1];
                    tmp = arrangeByte(_theoryValue[6]->text().toInt()+40,2);
                    b.value6[0] = tmp[0];
                    b.value6[1] = tmp[1];
                }
                if(property("number").toInt()>7)
                {
                    tmp = arrangeByte(_sampleValue[7]->text().toInt(),2);
                    b.sample7[0] = tmp[0];
                    b.sample7[1] = tmp[1];
                    tmp = arrangeByte(_theoryValue[7]->text().toInt()+40,2);
                    b.value7[0] = tmp[0];
                    b.value7[1] = tmp[1];
                }

                tmp=QByteArray::fromRawData((const char*)&b,sizeof(b)-(_MAX_MADJUST_POINT_-property("number").toInt())*4);
                ((CApp*)qApp)->_tjob->sendTestData232(tmp);
            }
        }
        else if(a._command == _METER_FRAME_RSAVE_PARA_WTEMP_ADJUST_)
        {
            if(property("read").toInt()==1)
            {
                adjustSPResponseFrame_0x52_0x55 rt = frame52_55FromData(d);

                if(rt.number <= _MAX_MADJUST_POINT_ && rt.number>0)
                {
                    vu16    value;
                    v16     sample;

	                for(int j=0;j<rt.number;j++)
	                {
                        value[j] = rt.value[j+rt.start]-40;
	                    sample[j] = rt.sample[j+rt.start];
	                }

	                    updateInterface(rt.number,value,sample);
                }
                else
                {
                    setProperty("read",-1);
                    newMessage(tr("参数读取错误，请确定仪表内有正确参数！"),0);
                }
            }
            else if(property("read").toInt()==0)
            {
                _savaStatusLabel->setText(tr("参数保存结束！"));
                setProperty("read",-1);

                for(int i=0;i<_MAX_MADJUST_POINT_;i++)
                {
                    _adjustButton[i]->setEnabled(true);
                    _currentStep[i]->setEnabled(true);
                }
                _zeroButton->setEnabled(true);
                _savaAllAdjustButton->setEnabled(true);
                _writeButton->setEnabled(true);

            }
        }
        else if(a._command == _METER_FRAME_CRESPONSE_ADJUST_)
        {
            if( property("zero").toInt()==1)
            {
                sendFrame(0x3,0x1,0x0);
                setProperty("zero",0);
                setProperty("wait0",1);
                _zeroButton->setEnabled(true);
            }
            else
            {
                adjustCResponseFrame_0x50 rt = frame50FromData(d);
                if(property("wait0").toInt()&&rt.sample!=0)
                {

                }
                else if(property("wait0").toInt()&&rt.sample==0)
                {
                    setProperty("wait0",0);
                }
                else
                {
                    _sampleValue[property("id").toInt()]->setText(QString::number(rt.sample));
                }
            }
        }
        else if(a._command == _METER_FRAME_RSAVE_ADJUST_)
        {
            adjustRSaveFrame_0x5F rt = frame5FFromData(d);
            _savaStatusLabel->setText(tr("参数固化入仪表成功！用时 ") + rt.time + tr(" ms"));
        }
    }
}

void CAdjustTestWaterTemperatureWindow::saveAllAdjustButtonClicked()
{
    setProperty("read",0);
    setProperty("testFlag",0);

    adjustInitParaFrame_0x41 b={_METER_FRAME_INIT_PARA_ADJUST_,0x00};
    b.type = 0x2;
    b.num_meter[2] = property("number").toInt();

    writeAdjustInitFrame(b);

    _savaAllAdjustButton->setEnabled(false);
    _savaStatusLabel->setText(tr("参数保存中..."));

    for(int i=0;i<_MAX_MADJUST_POINT_;i++)
    {
        _adjustButton[i]->setEnabled(false);
        _currentStep[i]->setEnabled(false);
        _zeroButton->setEnabled(false);
        _savaAllAdjustButton->setEnabled(false);
        _writeButton->setEnabled(false);
    }
    QByteArray tmp;
    tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
    ((CApp*)qApp)->_tjob->sendTestData232(tmp);
}

void CAdjustTestWaterTemperatureWindow::writeAdjustInitFrame(adjustInitParaFrame_0x41 &b)
{
    b.num_meter[0] = cf->value_meter["oil"].size();
    b.num_meter[1] = cf->value_meter["rspeed"].size();
    b.num_meter[2] = cf->value_meter["wtemp"].size();
    b.num_meter[3] = cf->value_meter["speed"].size();

    for(int i=0;i<cf->type_ana.size();i++)
        b.num_analog[i] = cf->value_ana[i].size();

    b.rcof = cf->oil_rcof;
    //***date,version,version1;
    QByteArray dd = getDateBCD();
    b.date[0] = dd[0];
    b.date[1] = dd[1];
    b.date[2] = dd[2];
    b.date[3] = dd[3];
}

void CAdjustTestWaterTemperatureWindow::writeButtonClicked()
{
    int iRet = QMessageBox::question(this, "警告", "确定把水温表调校参数（同时包括其他所有调校项目的参数）写入仪表？",
                                     QMessageBox::Yes, QMessageBox::No);
    if(iRet == QMessageBox::No)
        return;
    else
    {
        setProperty("testFlag",0);
        setProperty("read",0);

        adjustSaveFrame_0x4F b={_METER_FRAME_SAVE_ADJUST_,0x00};

        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        cf->writeFile( ((CApp*)qApp)->_config->_afile );
        setProperty("testFlag",0);
    }
}

void CAdjustTestWaterTemperatureWindow::readButtonClicked()
{
    int iRet = QMessageBox::question(this, "警告", "确定把仪表当前水温表调校参数读入？",
                                     QMessageBox::Yes, QMessageBox::No);
    if(iRet == QMessageBox::No)
        return;
    else
    {
        setProperty("testFlag",0);
        setProperty("read",1);

        adjustInitParaFrame_0x41 b={_METER_FRAME_INIT_PARA_ADJUST_,0x00};
        b.type = 0x01;

        QByteArray tmp=QByteArray::fromRawData((const char*)&b,sizeof(b));
        ((CApp*)qApp)->_tjob->sendTestData232(tmp);

        _savaStatusLabel->setText(tr("参数读取中..."));

        for(int i=0;i<_MAX_MADJUST_POINT_;i++)
        {
            _theoryValue[i]->setText("");
            _adjustButton[i]->setEnabled(false);
            _sampleValue[i]->setText("");
            _currentStep[i]->setEnabled(false);
        }
        _zeroButton->setEnabled(false);
        _savaAllAdjustButton->setEnabled(false);
        _writeButton->setEnabled(false);
    }
}

void CAdjustTestWaterTemperatureWindow::updateInterface(int number,vu16 value,v16 sample)
{
    int i;
    setProperty("number",number);
    setProperty("testFlag",0);
    setProperty("id",0);
    _centerGrid->removeWidget(_currentItem);
    _centerGrid->addWidget(_currentItem,3,property("id").toInt()+1);

    //***读取调校参数
    for(i=0;i<number;i++)
    {
        _theoryValue[i]->setText(QString::number(value[i]));//理论采样值
        _sampleValue[i]->setText(QString::number(sample[i]));//仪表采样值
        _currentStep[i]->setValue(sample[i]);
    }

    for(i=number;i<_MAX_MADJUST_POINT_;i++)
    {
        _adjustButton[i]->setEnabled(false);
        _theoryValue[i]->setText("");
        _adjustButton[i]->setEnabled(false);
        _sampleValue[i]->setText("");
        _currentStep[i]->setEnabled(false);
    }

    if(property("read").toInt()==1)
    {
        _savaStatusLabel->setText(tr("参数读取结束！"));

        for(i=0;i<number;i++)
        {
            _currentStep[i]->setEnabled(true);
            _adjustButton[i]->setEnabled(true);
        }
        _zeroButton->setEnabled(true);
        _savaAllAdjustButton->setEnabled(true);
        _writeButton->setEnabled(true);
        _readButton->setEnabled(true);
        setProperty("read",-1);
    }
}

/*快捷键*/
void CAdjustTestWaterTemperatureWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {

        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_A)
        {
            setProperty("testFlag",1);
            if(property("id").toInt() == 0)
            {
                setProperty("id", property("number").toInt() - 1);
            }
            else
                setProperty("id", property("id").toInt() - 1);
            _currentStep[property("id").toInt()]->setFocus();
            sendFrame(0x3,0x1,_currentStep[property("id").toInt()]->value());
            _centerGrid->removeWidget(_currentItem);
            _centerGrid->addWidget(_currentItem, 3, property("id").toInt() + 1);
        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_D)
        {
            setProperty("testFlag",1);
            if(property("id").toInt() == property("number").toInt() - 1)
            {
                setProperty("id", 0);
            }
            else
                setProperty("id", property("id").toInt() + 1);
            _currentStep[property("id").toInt()]->setFocus();
            sendFrame(0x3,0x1,_currentStep[property("id").toInt()]->value());
            _centerGrid->removeWidget(_currentItem);
            _centerGrid->addWidget(_currentItem, 3, property("id").toInt() + 1);
        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_W)
        {
            increaseButtonClicked();
        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_S)
        {
            decreaseButtonClicked();
        }
        else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S)
        {
            saveAllAdjustButtonClicked();
        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_R)
        {
            readButtonClicked();
        }
        else if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Z)
        {
            zeroButtonClicked();
        }
    }
}
