#ifndef CINITTESTWINDOW_H
#define CINITTESTWINDOW_H

#include <QWidget>
#include <QtGui>

class CInitTestWindow : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE CInitTestWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // CINITTESTWINDOW_H
