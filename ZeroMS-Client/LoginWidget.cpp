#include <QMovie>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include "../public/OSettings.h"
#include "global.h"
#include "OClientCore.h"
#include "LoginWidget.h"
#include "ui_LoginWidget.h"

LoginWidget::LoginWidget():ui(new Ui::LoginWidget),conn(0)
{
    ui->setupUi(this);

    qDebug()<<QImageReader::supportedImageFormats();

    QSize size(ui->Avatar->width(),ui->Avatar->height());
    QMovie *avatar = new QMovie(":/images/0ms2logo.png");
    avatar->setScaledSize(size);
    ui->Avatar->setMovie(avatar);
    avatar->start();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::cancel()
{
    if(conn)
        DELETE(conn);

    ui->DoLogin->setEnabled(true);
    ui->DoLogin->setText(tr("登录 >>"));
}

void LoginWidget::on_DoLogin_clicked()
{
    ui->DoLogin->setEnabled(false);
    ui->DoLogin->setText(tr("正在连接 ..."));

    QString uname=ui->UName->currentText();
    QString pwd=ui->Password->text();

    QTcpSocket *conn=new QTcpSocket;

    QEventLoop waitConnected;
    connect(conn,SIGNAL(connected()),&waitConnected,SLOT(quit()));
    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),&waitConnected,SLOT(quit()));

    qDebug()<<(*config)["SERVER_ADDRESS"].toString();
    conn->connectToHost(QHostAddress((*config)["SERVER_ADDRESS"].toString()),(*config)["SERVER_PORT"].toInt());

    waitConnected.exec();

    if(conn->state()!=QTcpSocket::ConnectedState)
    {
        QMessageBox::critical(this,tr("连接到服务器时发生错误"),conn->errorString());
        cancel();
        return;
    }

    core->main=new OServerPeer(conn);
    core->main->init();

    ui->DoLogin->setText(tr("获取公钥 ..."));
    QEventLoop waitPublicKey;
    connect(core->main,SIGNAL(onPublicKey(QString)),&waitPublicKey,SLOT(quit()));

    core->main->AskPublicKey();

    waitPublicKey.exec();

    ui->DoLogin->setText(tr("等待登录结果 ..."));

    core->main->Login(uname,OSha1(core->publicKey+OSha1(uname+OSha1(pwd))));
}
