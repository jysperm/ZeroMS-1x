#ifndef OPROTOCOLFORSC_H
#define OPROTOCOLFORSC_H

#include <QObject>
class QTcpSocket;

class OProtocolForSC:public QObject
{
    Q_OBJECT
public:
    explicit OProtocolForSC();

signals:

public slots:
    void checkMsg(QString uname,QTcpSocket *conn,QByteArray *databuf);
};

#endif // OPROTOCOLFORSC_H
