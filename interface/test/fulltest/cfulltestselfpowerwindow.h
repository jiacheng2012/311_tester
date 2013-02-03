#ifndef CFULLTESTSELFPOWERWINDOW_H
#define CFULLTESTSELFPOWERWINDOW_H

#include <QWidget>
#include <QtGui>
#include "configure/cmeterconfig.h"

class CFullTestSelfPowerWindow : public QWidget
{
    Q_OBJECT

        public:
            Q_INVOKABLE CFullTestSelfPowerWindow(QWidget *parent = 0);

            QLabel      *_testResult;
            QPushButton *_startButton;
            CMeterConfig *cf;

            QLabel      *_label1,*_label2,*_statusLabel;
            QTimer      *_timer;

        signals:
            void newMessage(QString msg, bool showOnScreen=0);
            void nextList(int);

        public slots:
            void sendBackData(QByteArray);
            void startButtonClicked();
            void timerUpdate();
};

#endif
