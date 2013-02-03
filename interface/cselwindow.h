#ifndef SELWINDOW_H
#define SELWINDOW_H

#include <QWidget>
#include <QtGui>

class CSelWindow : public QWidget
{
    Q_OBJECT

public:
    CSelWindow(QWidget *parent = 0);
    ~CSelWindow();

    QPushButton *testButton;
    QPushButton *configButton;
    QPushButton *databaseButton;

    QVBoxLayout *topLayout;

private:

public slots:

private slots:

};

#endif // SELWINDOW_H
