#include <QApplication>
#include <QDesktopServices>
#include <QEventLoop>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QString>
#include <QTcpSocket>
#include <QUrl>
#include <QWidget>
#include "const.h"
#include "login.h"
#include "ui_login.h"
#include "oclientcoreex.h"

extern OClientCoreEx *cc;

//public:
Login::Login(QWidget *parent):QWidget(parent),ui(new Ui::Login)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("%1-登录  服务器 %2").arg(CLIENT_TITLE_NAME).arg((cc->config)["SERVER_ADDRESS"].toString()));
    ui->LVer->setText(tr("版本 %1").arg(CLIENT_VER_NAME));
    //窗口居中
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    //绑定QLable点击信号的槽
    connect(ui->RegLink,SIGNAL(linkActivated(QString)),this,SLOT(QLable_linkActivated(QString)));
    connect(ui->ForgetLink,SIGNAL(linkActivated(QString)),this,SLOT(QLable_linkActivated(QString)));
    //设置网址
    ui->RegLink->setText(ui->RegLink->text().arg((cc->config)["REG_URL"].toString()));
    ui->ForgetLink->setText(ui->ForgetLink->text().arg((cc->config)["FORGET_URL"].toString()));

    ui->Banner->setStyleSheet("border-image:url(:/images/banner.png)");
}

Login::~Login()
{
    delete ui;
}

//public slots:
void Login::cancel()
{
    ui->DoLogin->setEnabled(1);
}

//private slots:
void Login::QLable_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void Login::LoginError()
{
    QMessageBox::critical(this,tr("登录错误"),tr("可能是你的密码输入错误\n如果你确认你的密码没有错误，请重试"));
    cancel();
}

void Login::on_DoLogin_clicked()
{
    ui->DoLogin->setEnabled(0);

    QString uname=ui->UserInput->text();
    QString pwd=ui->PassWordInput->text();

    QEventLoop waitConnected;
    connect(cc,SIGNAL(onConnected()),&waitConnected,SLOT(quit()));
    connect(cc,SIGNAL(onError(OClientCore::ErrorType,QString,QAbstractSocket::SocketError)),&waitConnected,SLOT(quit()));
    cc->connectTo((cc->config)["SERVER_ADDRESS"].toString(),(cc->config)["SERVER_PORT"].toInt());
    waitConnected.exec();
    if(cc->conn->state()!=QTcpSocket::ConnectedState)
    {
        cancel();
        return;
    }

    QEventLoop waitTimeUp;
    connect(cc,SIGNAL(onTimeChange(uint)),&waitTimeUp,SLOT(quit()));
    cc->msgAskTime();
    waitTimeUp.exec();

    cc->msgLogin(uname,pwd);
}

void Login::on_Options_clicked()
{
    QDesktopServices::openUrl(QUrl(PUBLIC_CONFIG_FILE));
}
