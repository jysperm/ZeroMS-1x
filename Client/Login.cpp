#include "const.h"
#include "login.h"
#include "ui_Login.h"
#include "OClientCoreEx.h"

extern OClientCoreEx *cc;

//public:
Login::Login(QWidget *parent):QWidget(parent),ui(new Ui::Login),isRemembered(0)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    //窗口居中
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);
    //绑定QLable点击信号的槽
    connect(ui->RegLink,SIGNAL(linkActivated(QString)),this,SLOT(QLable_linkActivated(QString)));
    connect(ui->ForgetLink,SIGNAL(linkActivated(QString)),this,SLOT(QLable_linkActivated(QString)));

    reSetUi();
}

Login::~Login()
{
    delete ui;
}

void Login::reSetUi()
{
    this->setWindowTitle(tr("%1-登录  服务器 %2").arg(CLIENT_TITLE_NAME).arg((cc->config)["SERVER_ADDRESS"].toString()));
    ui->LVer->setText(tr("版本 %1").arg(CLIENT_VER_NAME));

    //设置网址
    ui->RegLink->setText(ui->RegLink->text().arg((cc->config)["REG_URL"].toString()));
    ui->ForgetLink->setText(ui->ForgetLink->text().arg((cc->config)["FORGET_URL"].toString()));

    ui->Banner->setStyleSheet("border-image:url(:/images/banner.png)");

    //记住密码
    QSettings pwdIni((cc->config)["REMEMBERED_PATH"].toString(),QSettings::IniFormat);
    if(!pwdIni.value("REMEMBER_UNAME").toString().isEmpty() && !pwdIni.value("REMEMBER_PWD").toString().isEmpty())
    {
        ui->UserInput->setText(pwdIni.value("REMEMBER_UNAME").toString());
        ui->PassWordInput->setText(pwdIni.value("REMEMBER_PWD").toString());

        //设置显示的密码位数，要确保它必须是正整数
        int pwdLen=(pwdIni.value("REMEMBER_PWD_LEN").toInt() > 0)?pwdIni.value("REMEMBER_PWD_LEN").toInt():cc->config.defaultConfig->value("DEFAULT_PWD_LEN").toInt();
        ui->PassWordInput->setText(QString(pwdLen,QChar('*')));

        ui->Remember->setChecked(1);

        //当文本修改时，不再使用记住的密码
        connect(ui->PassWordInput,SIGNAL(textEdited(QString)),this,SLOT(on_UserInput_textEdited(QString)));
        connect(ui->UserInput,SIGNAL(textEdited(QString)),ui->PassWordInput,SLOT(clear()));
    }
}

void Login::onLoginOK()
{
    QSettings pwdIni((cc->config)["REMEMBERED_PATH"].toString(),QSettings::IniFormat);
    if(!ui->Remember->isChecked())
    {
        pwdIni.setValue("REMEMBER_UNAME",tr(""));
        pwdIni.setValue("REMEMBER_PWD",tr(""));
        pwdIni.setValue("REMEMBER_PWD_LEN",tr(""));
    }
    else
    {
        pwdIni.setValue("REMEMBER_UNAME",ui->UserInput->text());
        if(!isRemembered)
        {
            pwdIni.setValue("REMEMBER_PWD",ui->PassWordInput->text());
        }
    }
    pwdIni.sync();
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

    QString pwd;
    QSettings pwdIni((cc->config)["REMEMBERED_PATH"].toString(),QSettings::IniFormat);
    if(isRemembered)
        pwd=pwdIni.value("REMEMBER_PWD").toString();
    else
        pwd=ui->PassWordInput->text();

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
    
    cc->msgLogin(uname,pwd);
}

void Login::on_Options_clicked()
{
    QDesktopServices::openUrl(QUrl("./config.ini"));
}

void Login::on_UserInput_textEdited(const QString &text)
{
    isRemembered = 0;
}
