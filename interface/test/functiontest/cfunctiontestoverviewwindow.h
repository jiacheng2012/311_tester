#ifndef CFUNCTIONTESTOVERVIEWWINDOW_H
#define CFUNCTIONTESTOVERVIEWWINDOW_H

#include <QWidget>
#include <QtGui>

class CFunctionTestOverviewWindow : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE CFunctionTestOverviewWindow(QWidget *parent = 0);

    QPushButton *_485Button,*_232Button,*_nextButton;

    int _connected;

signals:
    void newMessage(QString msg, bool showOnScreen=0);
    void nextList(int);

public slots:
    void _232ButtonClicked();
    void _232Setup();
    void _485ButtonClicked();
    void _485Setup();
    void nextButtonClicked();
    void stop232();
    void stop485();
    
};

#endif
