#ifndef OSERVERCORE_H
#define OSERVERCORE_H

#include <QObject>
#include <QTextStream>
#include <QTcpServer>
#include "../public/OSettings.h"
#include "global.h"
#include "OClientConn.h"

class OServerCore:public QObject
{
    Q_OBJECT
public:
    explicit OServerCore();
    void init();
    void start();
    void checkMsg(QString uname,QTcpSocket *conn,QByteArray *databuf);

private:
    void log(QString msg);
    QMap<QString,OClientConn*> cl;
    QTextStream cin;
    QTextStream cout;
    QTcpServer server;
    OSettings config;
signals:

private slots:
    void onNewConn();
};

#endif // OSERVERCORE_H
