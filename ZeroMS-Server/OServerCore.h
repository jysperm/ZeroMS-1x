#ifndef OSERVERCORE_H
#define OSERVERCORE_H

#include <QObject>
#include <QTextStream>
#include <QTcpServer>
#include <QStringList>
#include "OServerDataBase.h"
#include "../public/OSettings.h"
#include "global.h"
#include "OClient.h"

class OServerCore:public QObject
{
    Q_OBJECT
    friend class OProtocolForSC;
    friend class OClientPeer;
public:
    explicit OServerCore();
    void init();
    void start();
private:
    QString getUserStatus(QString uname);
    void userListChange(QString uname);
    void log(QString msg);
    QMap<QString,OClient*> cl;
    QTextStream cin;
    QTextStream cout;
    QTcpServer server;
    OServerDataBase db;
    QMap<QString,QString> info;
private slots:
    void onNewConn();
    void onError(OClient *client);
};

#endif // OSERVERCORE_H
