#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QApplication>
#include <QDir>
#include <QEventLoop>
#include "odowner.h"

ODowner::ODowner(QObject *parent,int autoExit,int autoDelete):QThread(parent),isAutoExit(autoExit),isAutoDelete(autoDelete),manager(0),reply(0)
{

}

void ODowner::addFile(FileAddress address)
{
    list.append(address);
}

void ODowner::run()
{
    manager=new QNetworkAccessManager(this);
    while(!list.empty())
    {
        FileAddress fa=list.front();
        list.pop_front();

        QNetworkRequest request;
        request.setUrl(fa.url);
        reply=manager->get(request);

        QEventLoop waitDownload;
        connect(reply, SIGNAL(finished()), &waitDownload, SLOT(quit()));
        waitDownload.exec();

        //建立相应目录，和OClientCoreEx::mkDir功能是一样的，
        //但是考虑到该类不应该依赖OClientCoreEx，所以没直接调用
        QDir().mkpath(QFileInfo(fa.local).dir().path());

        QFile file(fa.local);
        file.open(QIODevice::WriteOnly);
        file.write(reply->readAll());
        file.close();

        emit finish(fa);
        if(list.empty())
            emit allFinish();

        //如果下载列表为空，且设置了isAutoExit，则等待列表中出现新文件
        while(!isAutoExit)
        {
            if(!list.empty())
                break;
            qApp->processEvents();
        }
    }
    if(isAutoDelete)
        delete this;
}

