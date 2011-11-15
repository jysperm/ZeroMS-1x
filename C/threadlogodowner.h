#ifndef THREADLOGODOWNER_H
#define THREADLOGODOWNER_H

class QApplication;
class QNetworkReply;
class QNetworkAccessManager;
#include <QThread>
#include <QStringList>

class ThreadLogoDowner:public QThread
{
    Q_OBJECT
public:
    ThreadLogoDowner(QStringList,QApplication*);
protected:
    void run();
signals:
    void finishedDown();
private slots:
    void ReadyRead();
private:
    QApplication *app;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QStringList users;
    int waiting;
};

#endif // THREADLOGODOWNER_H
