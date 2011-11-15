#include "threadlogodowner.h"
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttp>
#include "const.h"
#include "inline.h"

ThreadLogoDowner::ThreadLogoDowner(QStringList userlist,QApplication *a):app(a)
{
    users=userlist;
}

void ThreadLogoDowner::run()
{
    manager=new QNetworkAccessManager(this);
    QString i;
    foreach(i,users)
    {
	QNetworkRequest request;
	request.setUrl(QUrl(QString("http://jybox.net/0-ms/api/logo.php?uname=%1").arg(i)));
	qDebug()<<QString("http://jybox.net/0-ms/api/logo.php?uname=%1").arg(i);
	reply=manager->get(request);
	waiting=1;
	connect(reply, SIGNAL(finished()), this, SLOT(ReadyRead()));

	while(waiting)
	    app->processEvents();

	QFile logo((QDir::currentPath()+"/cahce/logo/%1.png").arg(i));
	logo.open(QIODevice::WriteOnly);
	logo.write(reply->readAll());
	logo.close();
	sDelete(reply);
    }
    emit finishedDown();
}

void ThreadLogoDowner::ReadyRead()
{
    waiting=0;
}
