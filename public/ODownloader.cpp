#include "ODownloader.h"

ODownloader::ODownloader(QObject *parent,int autoExit,int autoDelete):QThread(parent),isAutoExit(autoExit),isAutoDelete(autoDelete),manager(0),reply(0)
{
    //注册该类型，以便可以在信号槽中作为参数传递
    //与此对应的还有ODownloader.h中结尾的Q_DECLARE_METATYPE(ODowner::FileAddress)
    qRegisterMetaType<ODownloader::FileAddress>("ODowner::FileAddress");
}

void ODownloader::addFile(FileAddress address)
{
    list.append(address);
    emit newFile();
}

void ODownloader::run()
{
    manager=new QNetworkAccessManager;
    while(1)
    {
        //如果下载列表为空，且设置了isAutoExit，则等待列表中出现新文件
        while(!isAutoExit)
        {
            if(!list.empty())
                break;
            QEventLoop waitNewFile;
            connect(this, SIGNAL(newFile()), &waitNewFile, SLOT(quit()));
            waitNewFile.exec();
        }
        if(list.empty())
            break;

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

    }
    if(isAutoDelete)
        delete this;
}

