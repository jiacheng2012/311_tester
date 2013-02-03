#ifndef CFULLTESTFREQWINDOW_H
#define CFULLTESTFREQWINDOW_H

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QTextEdit>

#include "protocol/protocol.h"
#include "configure/cmeterconfig.h"

class CFullTestFreqWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFullTestFreqWindow(QWidget *parent = 0);

public:
        QSpinBox    *_standFreqVal;
        QLabel      *_channelSampleVal0 ;
        QLabel      *_channelSampleVal1;
        QPushButton *_testButton;

        CMeterConfig           *cf;
        QLabel      *_statusLabel;

        void setMap(PLCCommandFrame_0x80* a,int val);

signals:
        void newMessage(QString msg, bool showOnScreen=0);
        void nextList(int);

public slots:
        void sendBackData232(QByteArray);
        void sendBackData485(QByteArray);
        void testButtonClicked();
        void freqValueChanged(int val);
};

#endif
