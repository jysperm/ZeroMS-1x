#include "const.h"
#include "OServerCore.h"

//标准输入和标准输出
QTextStream cin(stdin);
QTextStream cout(stdout);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    OServerCore sc;
    sc.run();

    return a.exec();
}
