#include "csystembutton.h"
#include <QPainter>

csystembutton::csystembutton(QString picture,QString tooltip,QWidget *parent) :
    QPushButton(parent)
{
    mImage.load(picture);
    setWindowOpacity(0);
    setFlat(true);

    setToolTip(tooltip);
    mStatus= NORMAL;
}
void csystembutton::enterEvent(QEvent *)
{
    mStatus = ENTER;
    update();
}
void csystembutton::mousePressEvent(QMouseEvent *)
{
    mStatus = PRESS;
    update();
}
void csystembutton::mouseReleaseEvent(QMouseEvent *)
{
    mStatus = ENTER;
    update();
    emit clicked();
}
void csystembutton::leaveEvent(QEvent *)
{
    mStatus = NORMAL;
    update();
}

void csystembutton::paintEvent(QPaintEvent *)
{
    mWidth = mImage.width()/4;
    mHeight = mImage.height();
    setFixedSize(mWidth,mHeight);
    QPainter painter(this);
    painter.drawImage(rect(),mImage.copy(mWidth*mStatus,0,mWidth,mHeight));
}
