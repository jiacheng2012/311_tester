#ifndef CONFIG_CHANNEL_WINDOW_H
#define CONFIG_CHANNEL_WINDOW_H

#include <QWidget>
#include <QtGui>
#include "cconfigchanneltemplete.h"

class CConfigChannelWindow : public QWidget
{
    Q_OBJECT

public:
    CConfigChannelWindow(QWidget *parent = 0);
    ~CConfigChannelWindow();
private:

private:
    QTabWidget* _mainTab;
    QPushButton* _saveButton;
    QPushButton* _readButton;
protected:
    void keyReleaseEvent(QKeyEvent *event);
private slots:
    void    on__readButton_clicked();
    void    on__saveButton_clicked();
};

#endif // CONFIG_CHANNEL_WINDOW_H
