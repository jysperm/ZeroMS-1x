#include <QApplication>
#include "ONetworkDebuger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ONetworkDebuger w;
    w.show();
    
    return a.exec();
}
