#include <QApplication>
#include "ONetwokerDebuger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ONetwokerDebuger w;
    w.show();
    
    return a.exec();
}
