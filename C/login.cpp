#include <QApplication>
#include <QDesktopServices>
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
Login::Login(QWidget *parent):QWidget(parent),ui(new Ui::Login),exitLogin(0)
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
    //绑定OClientCoreEx的信号槽
    connect(cc,SIGNAL(onError(QString)),this,SLOT(socketError(QString)));
    connect(cc,SIGNAL(onLoginError()),this,SLOT(LoginError()));
    //设置网址
    ui->RegLink->setText(ui->RegLink->text().arg(REG_URL));
    ui->ForgetLink->setText(ui->ForgetLink->text().arg(FORGET_URL));

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

void Login::socketError(QString msg)
{
    QMessageBox::critical(this,tr("错误"),msg);
    exitLogin=1;
    cancel();
}

void Login::LoginError()
{
    QMessageBox::critical(this,tr("登录错误"),tr("可能是你的密码输入错误\n如果你确认你的密码没有错误，请重试"));
    cancel();
}

void Login::on_DoLogin_clicked()
{
    //其实这里设计的不大好，用了一堆乱七八糟的"标识变量"来控制阻塞
    //重构之前的代码也是这样的，我也实在想不到什么好主意了
    ui->DoLogin->setEnabled(0);
    exitLogin=0;

    QString uname=ui->UserInput->text();
    QString pwd=ui->PassWordInput->text();

    cc->connectTo(SERVER_ADDRESS,SERVER_PORT);

    while(!(cc->conn->state()==QTcpSocket::ConnectedState))
    {
        qApp->processEvents();
        if(exitLogin)
            return;
    }

    timeuped=0;
    cc->msgAskTime();
    while(!timeuped)
            qApp->processEvents();

    cc->msgLogin(uname,pwd);
}
