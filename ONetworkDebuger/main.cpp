#include <QApplication>
#include "ONetworkDebuger.h"

int main(int argc, char *argv[])
{
    QFont font;    qDebug()<<font.family();

    QApplication a(argc, argv);
    ONetworkDebuger w;
    w.show();
    
    return a.exec();
}
