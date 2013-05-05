#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZeroMS::Demo::RSATools::MainWindow w;
    w.show();
    
    return a.exec();
}
