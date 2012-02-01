#include <QTextCodec>
#include <QDesktopWidget>
#include <QDateTime>
#include "ONetwokerDebuger.h"
#include "ui_ONetwokerDebuger.h"

ONetwokerDebuger::ONetwokerDebuger(QWidget *parent):QWidget(parent),ui(new Ui::ONetwokerDebuger)
{
    ui->setupUi(this);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    updateList();
}

ONetwokerDebuger::~ONetwokerDebuger()
{
    delete ui;
}

void ONetwokerDebuger::updateList()
{
    ui->ConnectList->clear();
    QMapIterator<QString,UdpListen> iUdp(udpListenList);
    QString selectItem=ui->UdpSendSelect->currentText();
    ui->UdpSendSelect->clear();
    while(iUdp.hasNext())
    {
        iUdp.next();
        if(iUdp.value().isListen)
        {
            QString name=QString("%1:%2:%3").arg("UDP").arg(QHostAddress(QHostAddress::Any).toString()).arg(iUdp.value().conn->localPort());
            ui->ConnectList->insertItem(0,name);
        }
        ui->UdpSendSelect->addItem(QString::number(iUdp.value().conn->localPort()));
    }
    ui->UdpSendSelect->addItem(QString("新随机"));
    if(ui->UdpSendSelect->findText(selectItem)!=-1)
        ui->UdpSendSelect->setCurrentIndex(ui->UdpSendSelect->findText(selectItem));
}

void ONetwokerDebuger::on_DoUdpListen_clicked()
{
    int port=ui->UdpListenPort->value();
    QString name=QString("%1:%2:%3").arg("UDP").arg(QHostAddress(QHostAddress::Any).toString()).arg(port);
    if(udpListenList.contains(name))
    {
        if(!udpListenList[name].isListen)
        {
            udpListenList[name].isListen=true;
            log(QString("已存在端口为%1的发送对象，现已将其转换为监听对象\n监听%2").arg(port).arg(name));
        }
        else
        {
            log(QString("已存在端口为%1的监听对象").arg(port));
        }
    }
    else
    {
        UdpListen listener;
        listener.isListen=true;
        listener.conn=new QUdpSocket;
        listener.conn->bind(port);
        connect(listener.conn,SIGNAL(readyRead()),this,SLOT(onSocketNewData()));
        connect(listener.conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onSocketError(QAbstractSocket::SocketError)));
        udpListenList.insert(name,listener);
        log(QString("监听%1").arg(name));
    }
    updateList();
}

void ONetwokerDebuger::onSocketNewData()
{
    QMapIterator<QString,UdpListen> i(udpListenList);
    while(i.hasNext())
    {
        i.next();
        QUdpSocket *conn=i.value().conn;
        while(conn->hasPendingDatagrams())
        {
            QByteArray data;
            data.resize(conn->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            conn->readDatagram(data.data(), data.size(),&sender, &senderPort);
            log(sqlitter);
            log(QString("收到来自%1:%2的消息:").arg(sender.toString()).arg(senderPort));

            int ver=QBtoint(data.mid(0,4));
            unsigned int len=QBtoint(data.mid(4,4));
            int type=QBtoint(data.mid(8,4));
            unsigned int time=QBtoint(data.mid(12,4));

            QString stime=QDateTime::fromTime_t(time).toString("yyyy-MM-dd hh:mm:ss");

            log(QString("Ver-%1 Len-%2 %3(%4) %5(%6)").arg(ver).arg(len).arg(num2String(type)).arg(type)
                .arg(stime).arg(time));
        }
    }
}

void ONetwokerDebuger::onSocketError(QAbstractSocket::SocketError s)
{

}
