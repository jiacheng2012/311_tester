#ifndef DATABASEWINDOW_H
#define DATABASEWINDOW_H

#include <QWidget>
#include <QtGui>

class CDatabaseWindow : public QWidget
{
    Q_OBJECT

public:
    CDatabaseWindow(QWidget *parent = 0);
    ~CDatabaseWindow();

    QTabWidget *group;

    QVBoxLayout *topLayout;

private:

public slots:

private slots:

};

#endif
