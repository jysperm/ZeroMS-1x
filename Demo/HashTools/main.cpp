#include <QApplication>
#include "MainWindow.h"

#if QT_VERSION <= 0x050000
#include <QTextCodec>
#endif

int main(int argc, char *argv[])
{

#if QT_VERSION <= 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    QApplication a(argc, argv);
    ZeroMS::Demo::HashTools::MainWindow w;
    w.show();
    
    return a.exec();
}
