#include <QApplication>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QTcpSocket>
#include <QString>
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

    this->setWindowTitle(tr("%1-登录").arg(CLIENT_TITLE_NAME));
    ui->LVer->setText(tr("版本 %1").arg(CLIENT_VER_NAME));
    //窗口居中
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    //绑定QLable点击信号的槽
    connect(ui->RegLink,SIGNAL(linkActivated(QString)),this,SLOT(QLable_linkActivated(QString)));
    connect(ui->ForgetLink,SIGNAL(linkActivated(QString)),this,SLOT(QLable_linkActivated(QString)));
    //设置网址
    ui->RegLink->setText(ui->RegLink->text().arg(REG_URL));
    ui->ForgetLink->setText(ui->ForgetLink->text().arg(FORGET_URL));

    ui->Banner->setStyleSheet("border-image: url(:/Images/images/banner.png)");
}

Login::~Login()
{
    delete ui;
}

//public slots:
void Login::cancel()
{
    cc->error=0;
    ui->DoLogin->setEnabled(1);
}

void Login::LoginError()
{
    QMessageBox::information(this,tr("登录"),tr("登录失败"));
}

//private slots:
void Login::QLable_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void Login::on_DoLogin_clicked()
{
    ui->DoLogin->setEnabled(0);

    QString uname=ui->UserInput->text();
    QString pwd=ui->PassWordInput->text();

    cc->connectTo(SERVER_ADDRESS,SERVER_PORT);
    if(!cc->conn || cc->error)
    {
        cc->app->processEvents();
        if(cc->error)
        {
            QMessageBox::critical(this,CLIENT_TITLE_NAME,tr("连接到服务器时发生错误\nps.很可能是服务器挂了"));
            cancel();
            return;
        }
    }

    if(!cc->conn)
    {
        QMessageBox::critical(this,CLIENT_TITLE_NAME,tr("连接到服务器时发生错误\nps.很可能是服务器挂了"));
        cancel();
        return;
    }

    while(!(cc->conn->state()==QTcpSocket::ConnectedState))
    {
        cc->app->processEvents();
        if(cc->error)
        {
            QMessageBox::critical(this,CLIENT_TITLE_NAME,tr("连接到服务器时发生错误\nps.很可能是服务器挂了"));
            cancel();
            return;
        }

    }

    cc->uptime=0;
    cc->msgAskTime();
    while(!cc->uptime)
        cc->app->processEvents();
    cc->msgLogin(uname,pwd);

}
