#include <QMovie>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QTimer>
#include <QMenu>
#include "../public/OSettings.h"
#include "global.h"
#include "OClientCore.h"
#include "LoginWidget.h"
#include "ui_LoginWidget.h"

LoginWidget::LoginWidget():ui(new Ui::LoginWidget),conn(0),isCanenl(false)
{
    ui->setupUi(this);

    ui->password->installEventFilter(this);

    qDebug()<<QImageReader::supportedImageFormats();

    QMenu *popMenu=new QMenu;
    popMenu->addAction(tr("重置窗口大小"),this,SLOT(reSetWidgetSize()));
    popMenu->addSeparator();
    Act_isShowIp=new QAction("隐藏IP(不使用P2P功能)",popMenu);
    Act_isShowIp->setCheckable(true);
    popMenu->addAction(Act_isShowIp);
    ui->toolButton->setMenu(popMenu);

    QSize size(ui->avatar->width(),ui->avatar->height());
    QMovie *avatar = new QMovie(":/images/0ms2logo.png");
    avatar->setScaledSize(size);
    ui->avatar->setMovie(avatar);
    avatar->start();

    reSetUI();
}

LoginWidget::~LoginWidget()
{
    saveWidgetSize();

    delete ui->toolButton->menu();

    delete ui->avatar->movie();
    delete ui;
}

void LoginWidget::destroyLink()
{
    if(conn)
        DELETE(conn);
    if(core->main)
        core->main->deleteLater();

    ui->doLogin->setEnabled(true);
    ui->doLogin->setText(tr("登录 >>"));

    isCanenl=true;
}

void LoginWidget::timeout()
{
    destroyLink();
    QMessageBox::critical(this,tr("登录超时"),tr("与服务器通信超过%1秒，请检查网络连接后重试").arg((*config)["LOGIN_TIMEOUT"].toInt()));
}

void LoginWidget::saveWidgetSize()
{
    QSettings settings;

    settings.setValue("UI/LoginWidget/pos",pos());
    settings.setValue("UI/LoginWidget/size",size());
    settings.setValue("UI/LoginWidget/isShowIP",!Act_isShowIp->isChecked());
    settings.sync();
}

void LoginWidget::reSetWidgetSize()
{
    resize(QSize(275,600));

    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    saveWidgetSize();
}

void LoginWidget::reSetUI()
{
    setWindowTitle(tr("登录 - 零毫秒 %1").arg(::VERSION));
    ui->verLab->setText(tr("服务器 %1").arg((*config)["SERVER_ADDRESS"].toString()));

    QSettings settings;
    move(settings.value("UI/LoginWidget/pos",pos()).toPoint());
    resize(settings.value("UI/LoginWidget/size",size()).toSize());
    Act_isShowIp->setChecked(!settings.value("UI/LoginWidget/isShowIP",true).toBool());
}

void LoginWidget::doLogin(bool isForce)
{
    ui->doLogin->setEnabled(false);
    ui->doLogin->setText(tr("正在连接 ..."));

    QString uname=ui->uname->currentText();
    QString pwd=ui->password->text();
    bool isShowIp=!Act_isShowIp->isChecked();

    core->uname=uname;
    isCanenl=false;

    conn=new QTcpSocket;

    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeout()));
    timer.start((*config)["LOGIN_TIMEOUT"].toInt() * 1000);

    //等待连接到服务器
    QEventLoop waitConnected;
    connect(conn,SIGNAL(connected()),&waitConnected,SLOT(quit()));
    connect(conn,SIGNAL(error(QAbstractSocket::SocketError)),&waitConnected,SLOT(quit()));

    conn->connectToHost(QHostAddress((*config)["SERVER_ADDRESS"].toString()),(*config)["SERVER_PORT"].toInt());

    waitConnected.exec();

    if(conn->state()!=QTcpSocket::ConnectedState)
    {
        QMessageBox::critical(this,tr("连接到服务器时发生错误"),conn->errorString());
        destroyLink();
        return;
    }

    if(isCanenl)
        return;

    //创建主连接对象
    core->main=new OServerPeer(conn);
    conn=0;
    core->main->init();

    //等待获取公钥
    ui->doLogin->setText(tr("获取公钥 ..."));
    QEventLoop waitPublicKey;
    connect(core->main,SIGNAL(onPublicKey(QString)),&waitPublicKey,SLOT(quit()));

    core->main->AskPublicKey();

    waitPublicKey.exec();

    if(isCanenl)
        return;

    //等待登录结果(LoginResult),接下来的逻辑将转到OServerPeer::onLoginResult()
    ui->doLogin->setText(tr("等待登录结果 ..."));

    core->main->Login(uname,OSha1(core->publicKey+OSha1(uname+OSha1(pwd))),QVector<int>(),true,isForce,isShowIp);
}

bool LoginWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->password && event->type()==QEvent::KeyPress)
    {
        int key=(static_cast<QKeyEvent*>(event))->key();
        if(key==Qt::Key_Return || key==Qt::Key_Enter)
        {
            on_doLogin_clicked();
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void LoginWidget::on_doLogin_clicked()
{
    doLogin();
}
