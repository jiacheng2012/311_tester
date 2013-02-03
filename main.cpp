#include "capp.h"
#include "qtextcodec.h"

int main(int argc, char *argv[])
{
    CApp app(argc, argv);

    //***Setup chinese output
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    //*** Load Qss
    QFile styleSheet(":/res/311_v1.qss");
    if (styleSheet.open(QIODevice::ReadOnly)) {
        qDebug("qss file loaded!");
        app.setStyleSheet(styleSheet.readAll());
    }
    //*** Install Qt Translator for Chinese
    QTranslator translator(0);
    translator.load(":/res/qt_zh_CN",".");
    app.installTranslator(&translator);

    CMainWindow window;
    window.show();

    //***QPushButton鼠标指针 形状修改
    QList<QPushButton *> pushButtons = window.findChildren<QPushButton *>();
    foreach(QPushButton* pb,pushButtons){
        pb->setCursor(Qt::PointingHandCursor);
    }
    //***
    return app.exec();
}
