#ifndef SYSTEMBUTTON_H
#define SYSTEMBUTTON_H

#include <QPushButton>

class csystembutton : public QPushButton
{
    Q_OBJECT
public:
    explicit csystembutton(QString picture,QString tooltip,QWidget *parent = 0);

private:
    enum ButtonStatus{NORMAL,ENTER,PRESS,NOSTATUS};
    ButtonStatus  mStatus;
    QImage mImage;

    int  mWidth;
    int  mHeight;

protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void leaveEvent(QEvent *);
};

#endif // SYSTEMBUTTON_H
