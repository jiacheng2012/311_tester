#ifndef CADJUSTTESTOVERVIEWWINDOW_H
#define CADJUSTTESTOVERVIEWWINDOW_H

#include <QWidget>
#include <QtGui>


class CAdjustTestOverviewWindow : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CAdjustTestOverviewWindow(QWidget *parent = 0);

    QPushButton *_232Button,*_485Button,*_nextButton;
    QGroupBox   *_secondGroup;

    int     _connected;

signals:
    void newMessage(QString,bool);
    void nextList(int);

public slots:
    void _232ButtonClicked();
    void _485ButtonClicked();
    void _232Setup();
    void _485Setup();
    void nextButtonClicked();
    void stop232();
    void stop485();

};

#endif // CADJUSTTESTOVERVIEWWINDOW_H
