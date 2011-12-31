#include <QApplication>
#include <QDir>
#include <QTextCodec>
#include <QImageReader>
#include "const.h"
#include "oclientcoreex.h"

//声明客户端通讯核心(扩展)
OClientCoreEx *cc;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    a.setQuitOnLastWindowClosed(0);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QApplication::addLibraryPath("./plugins");

    qDebug()<<QImageReader::supportedImageFormats();

    cc=new OClientCoreEx;
    cc->init();

    return a.exec();
}
