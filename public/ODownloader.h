#ifndef PUBLIC_ODOWNER_H
#define PUBLIC_ODOWNER_H

#include <QtCore>
#include <QtNetwork>

class ODownloader:public QThread
{
    Q_OBJECT
public:
    class FileAddress//一个简单的结构体
    {
    public:
        FileAddress(){}
        FileAddress(QString u,QString l,QString k):url(u),local(l),key(k){}
        QString url;//下载地址
        QString local;//本地存储地址
        QString key;//额外信息
    };

    //当autoDelete为1时，在线程退出后后会自动delete自身
    //注意，只有以动态内存声明该对象时，才能使用autoDelete
    //autoExit为是否在所有文件下载完后自动退出线程(只是退出线程，但不会销毁该对象)
    explicit ODownloader(QObject *parent=0,int autoExit=1,int autoDelete=0);
    //向下载列表里添加文件
    void addFile(FileAddress address);
    int isAutoExit;
    int isAutoDelete;

    //请调用基类的start()函数开始下载
protected:
    void run();
signals:
    void finish(ODownloader::FileAddress address);
    void allFinish();
    void newFile();
private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QVector<FileAddress> list;
};

//见构造函数中的注释
Q_DECLARE_METATYPE(ODownloader::FileAddress)

#endif // PUBLIC_ODOWNER_H
