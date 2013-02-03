#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class csystembutton;

class CTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit CTitleBar(QWidget *parent = 0);
    
signals:
    void windowToMax();
    void windowToMin();
    void windowToClose();
public slots:

protected:
    bool eventFilter(QObject *, QEvent *);
private:
    QPoint m_ptMouseDown;
    bool  m_bMouseDown;
public:

    csystembutton *m_maxButton;
    csystembutton *m_minButton;
    csystembutton *m_closeButton;
};
#endif
