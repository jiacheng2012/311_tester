#include "capp.h"
#include "processing/ctestjob.h"
#include "cfulltestkeywindow.h"
#include <QDebug>

CFullTestKeyWindow::CFullTestKeyWindow(QWidget *parent) :
    QWidget(parent)
{
    //****
    setProperty("testFlag",0);
    QVBoxLayout *top = new QVBoxLayout();

    //***ui component
    //按键名称F1,F2,F3,F4
    _statusLabel = new QLabel(tr("请操作仪表按键来检测键值是否正确"));
    _statusLabel->setStyleSheet("font:bold 14px;color:#0099FF;max-height:26px;min-height:26px;background:#CCFF99;");
    _f1Label = new QLabel(tr("F1"));
    _f2Label = new QLabel(tr("F2"));
    _f3Label = new QLabel(tr("F3"));
    _f4Label = new QLabel(tr("F4"));
    //标准按键值
    _f1KeyValue = new QLabel(tr("0xF1"));
    _f1KeyValue->setFrameShape(QFrame::StyledPanel);
    _f1KeyValue->setAlignment(Qt::AlignCenter);
    _f2KeyValue = new QLabel(tr("0xF2"));
    _f2KeyValue->setFrameShape(QFrame::StyledPanel);
    _f2KeyValue->setAlignment(Qt::AlignCenter);
    _f3KeyValue = new QLabel(tr("0xF3"));
    _f3KeyValue->setFrameShape(QFrame::StyledPanel);
    _f3KeyValue->setAlignment(Qt::AlignCenter);
    _f4KeyValue = new QLabel(tr("0xF4"));
    _f4KeyValue->setFrameShape(QFrame::StyledPanel);
    _f4KeyValue->setAlignment(Qt::AlignCenter);
    //当前按键
    _fCurrentKeyValue = new QLabel(tr("F"));
    _fCurrentKeyValue->setFrameShape(QFrame::StyledPanel);
    _fCurrentKeyValue->setAlignment(Qt::AlignCenter);
    _fCurrentKeyValue->setStyleSheet("background:wheat;min-height:30px;font:18px bold;color:#0099FF;border:1px solid blue;");
    //***layout
    _mainGroup = new QGroupBox(tr("按键测试"));
    QGridLayout *_mainGrid =  new QGridLayout(_mainGroup);
    _mainGrid->addWidget(_statusLabel,0,0,1,5);
    _mainGrid->addWidget(new QLabel(tr("按键名称")),1,0);
    _mainGrid->addWidget(_f1Label,1,1,1,1,Qt::AlignCenter);
    _mainGrid->addWidget(_f2Label,1,2,1,1,Qt::AlignCenter);
    _mainGrid->addWidget(_f3Label,1,3,1,1,Qt::AlignCenter);
    _mainGrid->addWidget(_f4Label,1,4,1,1,Qt::AlignCenter);

    _mainGrid->addWidget(new QLabel(tr("标准按键值")),2,0);
    _mainGrid->addWidget(_f1KeyValue,2,1,1,1,Qt::AlignCenter);
    _mainGrid->addWidget(_f2KeyValue,2,2,1,1,Qt::AlignCenter);
    _mainGrid->addWidget(_f3KeyValue,2,3,1,1,Qt::AlignCenter);
    _mainGrid->addWidget(_f4KeyValue,2,4,1,1,Qt::AlignCenter);

    _mainGrid->addWidget(new QLabel(tr("当前按键")),3,0);
    _mainGrid->addWidget(_fCurrentKeyValue,3,2);

    top->addWidget(_mainGroup,0, Qt::AlignLeft | Qt::AlignTop);
    top->setSizeConstraint(QLayout::SetFixedSize);
    top->setAlignment(Qt::AlignTop);
    setLayout(top);

    //***
    connect((CApp*)qApp,SIGNAL(sendBackFullTestData_key(QByteArray)),this,SLOT(sendBackData(QByteArray)));
    cf = ((CApp*)qApp)->_tjob->_mconfig;
}

void CFullTestKeyWindow::sendBackData(QByteArray a)
{
    hardwareTestRespondFrame_0x20 rt = frame20FromData(a);
    if(rt.key==0x71)
    {
        _fCurrentKeyValue->setText(tr("F1"));
        _f1Label->setStyleSheet("background-color:red");
    }
    else if(rt.key==0x70)
    {
        _fCurrentKeyValue->setText(tr("F2"));
        _f2Label->setStyleSheet("background-color:red");
    }
    else if(rt.key==0x72)
    {
        _fCurrentKeyValue->setText(tr("F3"));
        _f3Label->setStyleSheet("background-color:red");
    }
    else if(rt.key==0x73)
    {
        _fCurrentKeyValue->setText(tr("F4"));
        _f4Label->setStyleSheet("background-color:red");
    }
    else if(rt.key!=0x00)
        newMessage(tr("按键错误"));
    else
    {
        _f1Label->setStyleSheet("background-color:none");
        _f2Label->setStyleSheet("background-color:none");
        _f3Label->setStyleSheet("background-color:none");
        _f4Label->setStyleSheet("background-color:none");
    }
}
