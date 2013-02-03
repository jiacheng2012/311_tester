#ifndef CLOGINWINDOW_H
#define CLOGINWINDOW_H

#include <QWidget>
#include <QtGui>
#include "database/cuser.h"

class CLoginWindow : public QWidget
{
    Q_OBJECT
private:
    QLineEdit   *_userName,*_passWord;
    QLabel      *_errMsg;

public:
    explicit CLoginWindow(QWidget *parent = 0);
    
signals:
    void loginSuccess();
    void addUser(CUser&);

public slots:
    void loginButtonClicked();
};

#endif // CLOGINWINDOW_H
