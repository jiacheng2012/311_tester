#ifndef CFULLTESTLCDWINDOW_H
#define CFULLTESTLCDWINDOW_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QTextEdit>
#include <QLabel>
#include "configure/cmeterconfig.h"

class CFullTestLCDWindow : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE CFullTestLCDWindow(QWidget *parent = 0);

    QPushButton *_startButton;
    CMeterConfig           *cf;
    QLabel      *_statusLabel;


signals:
    void sendData(QByteArray,int);
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void sendBackData(QByteArray);
    void startButtonClicked();
};

#endif
