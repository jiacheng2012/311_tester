#ifndef CFULLTESTMETERVOLTAGEWINDOW_H
#define CFULLTESTMETERVOLTAGEWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

class CFullTestMeterVoltageWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CFullTestMeterVoltageWindow(QWidget *parent = 0);
private:
    QLabel *_result1,*_result2;
    QPushButton *_startButton;
    CMeterConfig           *cf;
    QComboBox    *_voltageBox;
    int         _val;
    QLabel      *_statusLabel;

signals:
    void sendBackFullTestData_metervoltage(QByteArray);
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void sendBackData232(QByteArray);
    void sendBackData485(QByteArray);
    void startButtonClicked();
    void indexChanged(int id);
};

#endif
