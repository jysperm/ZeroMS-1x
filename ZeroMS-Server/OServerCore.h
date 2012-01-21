#ifndef OSERVERCORE_H
#define OSERVERCORE_H

#include <QObject>
#include <QTextStream>
#include <QTcpServer>
#include "../public/OSettings.h"
#include "global.h"

class OServerCore:public QObject
{
    Q_OBJECT
public:
    explicit OServerCore();
    void init();
    void start();
protected:
    virtual void log(QString msg);
private:
    QTextStream cin;
    QTextStream cout;
    QTcpServer server;
    OSettings config;
signals:

private slots:
    void onNewConn();
};

#endif // OSERVERCORE_H
