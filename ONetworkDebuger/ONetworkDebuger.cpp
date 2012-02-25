#include <QTextCodec>
#include <QDesktopWidget>
#include <QDateTime>
#include <QMessageBox>
#include "ONetworkDebuger.h"
#include "ui_ONetworkDebuger.h"

ONetworkDebuger::ONetworkDebuger(QWidget *parent):QMainWindow(parent),ui(new Ui::ONetworkDebuger)
{
    ui->setupUi(this);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    updateList();
    on_UdpSendAutoCalcLength_clicked(true);
    on_UdpSendAutoFillCurrentTime_clicked(true);
    on_TcpSendAutoCalcLength_clicked(true);
    on_TcpSendAutoFillCurrentTime_clicked(true);
    on_UdpSendHeadMsgType_valueChanged(0);
    on_TcpSendHeadMsgType_valueChanged(0);
    on_UdpSendHeadTime_textChanged("0");
    on_UdpSendHeadTime_textChanged("0");
}

ONetworkDebuger::~ONetworkDebuger()
{
    on_DoInitialize_clicked();
    delete ui;
}

void ONetworkDebuger::updateList()
{
    ui->ConnectList->clear();
    QMapIterator<QString,UdpConnect> iUdp(udpList);
    QString selectItem=ui->UdpSendSelect->currentText();
    ui->UdpSendSelect->clear();
    while(iUdp.hasNext())
    {
        iUdp.next();
        if(iUdp.value().isListen)
        {
            ui->ConnectList->insertItem(0,iUdp.key());
        }
        ui->UdpSendSelect->addItem(QString::number(iUdp.value().conn->localPort()));
    }
    ui->UdpSendSelect->addItem(QString("新随机"));
    if(ui->UdpSendSelect->findText(selectItem)!=-1)
        ui->UdpSendSelect->setCurrentIndex(ui->UdpSendSelect->findText(selectItem));


    QMapIterator<QString,QTcpServer*> iTcpServer(tcpServerList);
    while(iTcpServer.hasNext())
    {
        iTcpServer.next();

        ui->ConnectList->insertItem(0,iTcpServer.key());
    }

    ui->TcpSendSelect->clear();
    QMapIterator<QString,QTcpSocket*> iTcpSocket(tcpList);
    while(iTcpSocket.hasNext())
    {
        iTcpSocket.next();

        ui->ConnectList->insertItem(0,iTcpSocket.key());
        ui->TcpSendSelect->addItem(iTcpSocket.key());
    }
}

void ONetworkDebuger::on_DoUdpListen_clicked()
{
    int port=ui->UdpListenPort->value();
    QString name=QString("%1:%2:%3").arg("UDP").arg(QHostAddress(QHostAddress::Any).toString()).arg(port);
    if(udpList.contains(name))
    {
        if(!udpList[name].isListen)
        {
            udpList[name].isListen=true;
            log(QString("已存在%1的发送对象，现已将其转换为监听对象").arg(name));
        }
        else
        {
            log(QString("已经存在监听%1的连接了").arg(name));
        }
    }
    else
    {
        UdpConnect listener;
        listener.isListen=true;
        listener.conn=new QUdpSocket;
        listener.conn->bind(port);
        connect(listener.conn,SIGNAL(readyRead()),this,SLOT(onSocketNewData()));
        connect(listener.conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onSocketError(QAbstractSocket::SocketError)));
        udpList.insert(name,listener);
        log(QString("监听%1").arg(name));
    }
    updateList();
}

void ONetworkDebuger::on_UdpDoSend_clicked()
{
    UdpConnect sender;
    QString sport=ui->UdpSendSelect->currentText();
    QString name=QString("%1:%2:%3").arg("UDP").arg(QHostAddress(QHostAddress::Any).toString()).arg(sport);

    if(sport==QString("新随机"))
    {
        sender.conn=new QUdpSocket;
        sender.isListen=false;
        sender.conn->bind();
        QString name=QString("%1:%2:%3").arg("UDP").arg(QHostAddress(QHostAddress::Any).toString()).arg(sender.conn->localPort());
        udpList.insert(name,sender);
    }
    else if(udpList.contains(name))
    {
        sender=udpList[name];
    }
    else
    {
        sender.conn=new QUdpSocket;
        sender.isListen=false;
        sender.conn->bind(sport.toInt());
        QString name=QString("%1:%2:%3").arg("UDP").arg(QHostAddress(QHostAddress::Any).toString()).arg(sender.conn->localPort());
        udpList.insert(name,sender);
    }

    QByteArray content;
    content.append(ui->UdpSendContent->toPlainText());

    if(ui->UdpSendAutoCalcLength->isChecked())
    {
        ui->UdpSendHeadLength->setValue(content.length());
    }
    if(ui->UdpSendAutoFillCurrentTime->isChecked())
    {
        ui->UdpSendHeadTime->setText(QString::number(QDateTime::currentDateTime().toTime_t()));
    }

    int ver=ui->UdpSendHeadVersion->value();
    unsigned int len=ui->UdpSendHeadLength->value();
    int type=ui->UdpSendHeadMsgType->value();
    unsigned int time=ui->UdpSendHeadTime->text().toInt();

    QByteArray data;
    data.append(inttoQB(ver));
    data.append(inttoQB(len));
    data.append(inttoQB(type));
    data.append(inttoQB(time));
    data.append(content);

    QString ip=ui->UdpSendIp->text();
    int port=ui->UdpSendPort->value();

    sender.conn->writeDatagram(data,QHostAddress(ip),port);

    if(ui->ShowSendMsg->isChecked())
    {
        QString stime=QDateTime::fromTime_t(time).toString("yyyy-MM-dd hh:mm:ss");

        log(sqlitter);
        log(QString("向UDP:%1:%2发送数据包:").arg(ip).arg(port));

        log(QString("Ver-%1 Len-%2 %3(%4) %5(%6)").arg(ver).arg(len).arg(num2String(type)).arg(type)
            .arg(stime).arg(time));

        log(QString(content));
    }

    updateList();
}

void ONetworkDebuger::on_TcpDoSend_clicked()
{
    QTcpSocket *sender;
    if(!tcpList.contains(ui->TcpSendSelect->currentText()))
        return;
    sender=tcpList[ui->TcpSendSelect->currentText()];

    QByteArray content;
    content.append(ui->TcpSendContent->toPlainText());

    if(ui->TcpSendAutoCalcLength->isChecked())
    {
        ui->TcpSendHeadLength->setValue(content.length());
    }
    if(ui->TcpSendAutoFillCurrentTime->isChecked())
    {
        ui->TcpSendHeadTime->setText(QString::number(QDateTime::currentDateTime().toTime_t()));
    }

    int ver=ui->TcpSendHeadVersion->value();
    unsigned int len=ui->TcpSendHeadLength->value();
    int type=ui->TcpSendHeadMsgType->value();
    unsigned int time=ui->TcpSendHeadTime->text().toInt();

    QByteArray data;
    data.append(inttoQB(ver));
    data.append(inttoQB(len));
    data.append(inttoQB(type));
    data.append(inttoQB(time));
    data.append(content);

    sender->write(data);

    if(ui->ShowSendMsg->isChecked())
    {
        QString stime=QDateTime::fromTime_t(time).toString("yyyy-MM-dd hh:mm:ss");

        log(sqlitter);
        log(QString("向%1发送数据包:").arg(ui->TcpSendSelect->currentText()));

        log(QString("Ver-%1 Len-%2 %3(%4) %5(%6)").arg(ver).arg(len).arg(num2String(type)).arg(type)
            .arg(stime).arg(time));

        log(QString(content));
    }

    updateList();
}

void ONetworkDebuger::on_DoTcpListen_clicked()
{
    int port=ui->TcpListenPort->value();
    QString name=QString("%1:%2:%3").arg("TCP").arg(QHostAddress(QHostAddress::Any).toString()).arg(port);

    if(tcpServerList.contains(name))
    {
        log(QString("已经存在监听%1的连接了").arg(name));
        return;
    }

    QTcpServer *server=new QTcpServer;
    connect(server,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
    server->listen(QHostAddress(QHostAddress::Any),port);

    tcpServerList.insert(name,server);

    updateList();
}

void ONetworkDebuger::onNewConnection()
{
    bool autoAccept=ui->AutoAccept->isChecked();

    QMapIterator<QString,QTcpServer*> iTcpServer(tcpServerList);
    while(iTcpServer.hasNext())
    {
        iTcpServer.next();

        QString name=QString("%1:%2:%3").arg("TCP").arg(QHostAddress(QHostAddress::Any).toString()).arg(iTcpServer.value()->serverPort());

        while(iTcpServer.value()->hasPendingConnections())
        {
            QTcpSocket *conn=iTcpServer.value()->nextPendingConnection();

            QString peerName=QString("%1:%2:%3").arg("TCP").arg(conn->peerAddress().toString()).arg(conn->peerPort());
            if(!autoAccept)
            {
                int r=QMessageBox::question(this,QString("有一个新TCP连接请求，是否接受"),
                                            QString("%1接收到了一个新连接请求%2").arg(name).arg(peerName),
                                            QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
                if(QMessageBox::Yes!=r)
                {
                    conn->abort();
                    continue;
                }
            }

            connect(conn,SIGNAL(readyRead()),this,SLOT(onSocketNewData()));
            connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onSocketError(QAbstractSocket::SocketError)));

            log(QString("在%1上接受了一个新连接%2").arg(name).arg(peerName));

            tcpList.insert(peerName,conn);
        }
    }

    updateList();
}

void ONetworkDebuger::on_DoTcpConnect_clicked()
{
    QTcpSocket *conn=new QTcpSocket;
    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(conn,SIGNAL(readyRead()),this,SLOT(onSocketNewData()));
    conn->connectToHost(QHostAddress(ui->TcpConnectIp->text()),ui->TcpConnectPort->value());

    QEventLoop wait;
    connect(conn,SIGNAL(connected()),&wait,SLOT(quit()));
    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),&wait,SLOT(quit()));
    wait.exec();

    QString name=QString("%4-%1:%2:%3").arg("TCP").arg(ui->TcpConnectIp->text()).arg(ui->TcpConnectPort->value()).arg(conn->localPort());
    tcpList.insert(name,conn);

    log(QString("正在连接到%1").arg(name));
    updateList();
}

void ONetworkDebuger::onSocketNewData()
{
    QMapIterator<QString,UdpConnect> i(udpList);
    while(i.hasNext())
    {
        i.next();
        QUdpSocket *conn=i.value().conn;
        if(!i.value().isListen)
            continue;
        while(conn->hasPendingDatagrams())
        {
            QByteArray data;
            data.resize(conn->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            conn->readDatagram(data.data(), data.size(),&sender, &senderPort);
            log(sqlitter);
            log(QString("收到来自UDP:%1:%2的数据包:").arg(sender.toString()).arg(senderPort));

            int ver=QBtoint(data.mid(0,4));
            unsigned int len=QBtoint(data.mid(4,4));
            int type=QBtoint(data.mid(8,4));
            unsigned int time=QBtoint(data.mid(12,4));

            QString stime=QDateTime::fromTime_t(time).toString("yyyy-MM-dd hh:mm:ss");

            log(QString("Ver-%1 Len-%2 %3(%4) %5(%6)").arg(ver).arg(len).arg(num2String(type)).arg(type)
                .arg(stime).arg(time));

            log(data.mid(16));
        }
    }

    QMapIterator<QString,QTcpSocket*> iTcpSocket(tcpList);
    while(iTcpSocket.hasNext())
    {
        iTcpSocket.next();
        QTcpSocket *conn=iTcpSocket.value();
        if(!conn->atEnd())
        {
            QByteArray data=conn->readAll();

            log(sqlitter);
            log(QString("TCP:%4收到来自TCP:%1:%2的数据包:").arg(conn->peerAddress().toString()).arg(conn->peerPort()).arg(conn->localPort()));

            int ver=QBtoint(data.mid(0,4));
            unsigned int len=QBtoint(data.mid(4,4));
            int type=QBtoint(data.mid(8,4));
            unsigned int time=QBtoint(data.mid(12,4));

            QString stime=QDateTime::fromTime_t(time).toString("yyyy-MM-dd hh:mm:ss");

            log(QString("Ver-%1 Len-%2 %3(%4) %5(%6)").arg(ver).arg(len).arg(num2String(type)).arg(type)
                .arg(stime).arg(time));

            log(data.mid(16));
        }
    }
}

void ONetworkDebuger::onSocketError(QAbstractSocket::SocketError s)
{
    QMapIterator<QString,UdpConnect> iUdp(udpList);
    while(iUdp.hasNext())
    {
        iUdp.next();
        if(iUdp.value().conn->error()==s)
        {
            log(QString("%1发生错误：%2").arg(iUdp.key()).arg(iUdp.value().conn->errorString()));
            delete iUdp.value().conn;
            udpList.remove(iUdp.key());
        }
    }

    QMapIterator<QString,QTcpSocket*> iTcpSocket(tcpList);
    while(iTcpSocket.hasNext())
    {
        iTcpSocket.next();
        if(iTcpSocket.value()->error()==s)
        {
            log(QString("%1发生错误：%2").arg(iTcpSocket.key()).arg(iTcpSocket.value()->errorString()));
            iTcpSocket.value()->abort();
            iTcpSocket.value()->~QTcpSocket();
            tcpList.remove(iTcpSocket.key());
        }
    }
    updateList();
}

void ONetworkDebuger::on_DoInitialize_clicked()
{
    QMapIterator<QString,UdpConnect> iUdp(udpList);
    while(iUdp.hasNext())
    {
        iUdp.next();
        delete iUdp.value().conn;
        udpList.remove(iUdp.key());
    }

    QMapIterator<QString,QTcpSocket*> iTcpSocket(tcpList);
    while(iTcpSocket.hasNext())
    {
        iTcpSocket.next();
        iTcpSocket.value()->abort();
        tcpList.remove(iTcpSocket.key());
    }

    QMapIterator<QString,QTcpServer*> iTcpServer(tcpServerList);
    while(iTcpServer.hasNext())
    {
        iTcpServer.next();
        delete iTcpServer.value();
        tcpServerList.remove(iTcpServer.key());
    }

    updateList();
}

void ONetworkDebuger::on_UdpSendAutoCalcLength_clicked(bool checked)
{
    ui->UdpSendHeadLength->setEnabled(!checked);
}

void ONetworkDebuger::on_UdpSendAutoFillCurrentTime_clicked(bool checked)
{
    ui->UdpSendHeadTime->setEnabled(!checked);
}

void ONetworkDebuger::on_TcpSendAutoCalcLength_clicked(bool checked)
{
    ui->TcpSendHeadLength->setEnabled(!checked);
}

void ONetworkDebuger::on_TcpSendAutoFillCurrentTime_clicked(bool checked)
{
    ui->TcpSendHeadTime->setEnabled(!checked);
}

void ONetworkDebuger::on_ConnectList_itemDoubleClicked(QListWidgetItem *item)
{
    if(udpList.contains(item->text()))
    {
        delete udpList[item->text()].conn;
        udpList.remove(item->text());
    }

    if(tcpServerList.contains(item->text()))
    {
        tcpServerList[item->text()]->close();
        tcpServerList.remove(item->text());
    }

    if(tcpList.contains(item->text()))
    {
        tcpList[item->text()]->close();
        tcpList.remove(item->text());
    }

    updateList();
}

void ONetworkDebuger::on_UdpSendHeadMsgType_valueChanged(int arg1)
{
    ui->UdpSendHeadMsgType->setToolTip(QString("%1\n修改后，悬停可看对应的消息名").arg(num2String(arg1)));
}

void ONetworkDebuger::on_TcpSendHeadMsgType_valueChanged(int arg1)
{
    ui->TcpSendHeadMsgType->setToolTip(QString("%1\n修改后，悬停可看对应的消息名").arg(num2String(arg1)));
}

void ONetworkDebuger::on_UdpSendHeadTime_textChanged(const QString &arg1)
{
    QString stime=QDateTime::fromTime_t(arg1.toInt()).toString("yyyy-MM-dd hh:mm:ss");
    ui->UdpSendHeadTime->setToolTip(QString("%1\n修改后，悬停可看时间字符串").arg(stime));
}

void ONetworkDebuger::on_TcpSendHeadTime_textChanged(const QString &arg1)
{
    QString stime=QDateTime::fromTime_t(arg1.toInt()).toString("yyyy-MM-dd hh:mm:ss");
    ui->TcpSendHeadTime->setToolTip(QString("%1\n修改后，悬停可看时间字符串").arg(stime));
}

void ONetworkDebuger::on_action_triggered()
{
    QStringList tL=ui->TcpSendContent->toPlainText().split(" ");
    QString u=tL[0];
    QString p=tL[1];
    QString k=tL[2];

    //SHA1(公钥+SHA1(用户名+SHA1(密码)))
    QString m=Osha1(k+Osha1(u+Osha1(p)));

    ui->TcpSendContent->setText(QString("%1 %2 / main force showip").arg(u).arg(m));
}

void ONetworkDebuger::on_action_SHA_1_triggered()
{
    QStringList tL=ui->TcpSendContent->toPlainText().split(" ");
    QString u=tL[0];
    QString p=tL[1];

    //SHA1(用户名+SHA1(密码))
    QString m=Osha1(u+Osha1(p));

    log(m);
}
