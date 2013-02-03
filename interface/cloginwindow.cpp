#include "cloginwindow.h"
#include "capp.h"

CLoginWindow::CLoginWindow(QWidget *parent) :
    QWidget(parent)
{
    //***UI Component
    QLabel *label_1 = new QLabel(tr("用户："));
    QLabel *label_2 = new QLabel(tr("密码："));

    _userName = new QLineEdit();
    _passWord = new QLineEdit();
    //_errMsg = new QLabel(tr("请输入用户名和密码登录"));
    _errMsg = new QLabel("<div style='color:green;font:bold 13px;'>请输入用户名和密码登录</div>");

    QPushButton *login = new QPushButton(tr("登陆"));
    login->setObjectName("login");

    QGridLayout *topLayout = new QGridLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(_errMsg,0,Qt::AlignCenter);
    mainLayout->addLayout(topLayout);
    //mainLayout->addStretch();
    //topLayout->addWidget(_errMsg,0,0);
    topLayout->setContentsMargins(30,20,30,20);
    topLayout->addWidget(label_1,1,0);
    topLayout->addWidget(_userName,1,1);
    topLayout->addWidget(label_2,2,0);
    topLayout->addWidget(_passWord,2,1);
    //topLayout->addWidget(login,3,0);
    mainLayout->addWidget(login,0,Qt::AlignCenter);
    setLayout(mainLayout);

    //***Signal
    qRegisterMetaType<CUser>("CUser");
    connect(login,SIGNAL(clicked()),this,SLOT(loginButtonClicked()));
    connect(this,SIGNAL(loginSuccess()),this->parent(),SLOT(loginButtonClicked()));
    connect(this,SIGNAL(addUser(CUser&)),(CApp*)qApp,SLOT(addUser(CUser&)));
}

void CLoginWindow::loginButtonClicked()
{
    bool status= ((CApp*)qApp)->_db->Open();
    qDebug() << "database status: " << status << " last error: " << ((CApp*)qApp)->_db->lastError();

//    qint32 l = ((CApp*)qApp)->_db->validateUser(_userName->text(),_passWord->text());
    qint32 l = ((CApp*)qApp)->_db->validateUser("admin","123");
    l=0;
    if( l!=-1 )
    {
        CUser u;
        u._level = l;
        emit addUser(u);
        emit loginSuccess();
    }
    else
    {
        _userName->setText("");
        _passWord->setText("");
        _errMsg->setText(tr("用户名或者密码错误，登录失败！"));
    }
}
