#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QTextCodec>
#include "const.h"
#include "oservercore.h"

#if defined Q_OS_LINUX  

#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>

#endif // Q_OS_LINUX  

//标准输入和标准输出
QTextStream cin(stdin);
QTextStream cout(stdout);

int main(int argc, char *argv[])
{       
    #if defined Q_OS_LINUX  
    int pid; 
    int i;
    if(pid=fork()) 
        exit(0);//是父进程，结束父进程 
    else if(pid< 0) 
        exit(1);//fork失败，退出 
    
    //是第一子进程，后台继续执行
    setsid();//第一子进程成为新的会话组长和进程组长并和控制终端分离 
    
    if(pid=fork()) 
        exit(0);//是第一子进程，结束第一子进程 
    else if(pid< 0) 
        exit(1);//fork失败，退出 
    
    //是第二子进程，继续 
    //第二子进程不再是会话组长
    for(i=0;i< NOFILE; i)//关闭打开的文档描述符 
        close(i); 
    chdir("/tmp");//改变工作目录到/tmp 
    umask(0);//重设文档创建掩模 
    #endif // Q_OS_LINUX  
        
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    OServerCore sc;
    sc.run();

    return a.exec();
}
