#include "const.h"
#include "OClientCoreEx.h"

OClientCoreEx *cc;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    a.setQuitOnLastWindowClosed(0);

    QApplication::addLibraryPath("./plugins");

    qDebug()<<QImageReader::supportedImageFormats();

    cc = new OClientCoreEx;
    cc->init();

    return a.exec();
}
