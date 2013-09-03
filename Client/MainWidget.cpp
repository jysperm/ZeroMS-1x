#include "OClientCoreEx.h"
#include "MainWidget.h"
#include "ui_MainWidget.h"

extern OClientCoreEx *cc;

//public:
MainWidget::MainWidget(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWidget),LEnterToSend(0)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    //窗口居中
    QDesktopWidget* desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);

    //托盘
    trayIcon=new QSystemTrayIcon(this);
    QIcon icon(":/images/40_0ms.png");
    trayMenu=new QMenu(this);
    trayMenu->addAction(ui->ActQuit);
    trayMenu->addSeparator();
    trayIcon->setIcon(icon);
    trayIcon->show();
    trayIcon->setContextMenu(trayMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));

    //菜单栏信号槽
    connect(ui->ActRefresh,SIGNAL(triggered()),cc,SLOT(msgAskUList()));
    connect(ui->ActMinSize, SIGNAL(triggered()), this, SLOT(showMinimized()));
    connect(ui->ActUserBBS, SIGNAL(triggered()), ui->ActDevBBS, SLOT(trigger()));
    connect(ui->ActSend, SIGNAL(triggered()), ui->DoSend, SLOT(click()));
    connect(ui->ActClear, SIGNAL(triggered()), ui->MsgArea, SLOT(clear()));
    connect(ui->ActABoutQt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));

    //回车发送
    LEnterToSend=new QLabel(this);
    statusBar()->addWidget(LEnterToSend);
    //为消息输入框安装事件过滤器
    ui->MsgEdit->installEventFilter(this);

    reSetUi();
}

MainWidget::~MainWidget()
{
    delete trayIcon;
    delete trayMenu;
    delete ui;
}

void MainWidget::reSetUi()
{
    ui->LMyName->setText(cc->myname);
    setWindowTitle(QString("%1  %2").arg(cc->myname).arg(CLIENT_TITLE_NAME));

    //这里有些蛋痛，其实不要也可以
    int enter=(cc->config)["ENTER_TO_SEND"].toInt();
    int ctrl=(cc->config)["CTRL_ENTER_TO_SEND"].toInt();
    if(enter && ctrl)
        LEnterToSend->setText(tr("回车和Ctrl+回车都可以发送哦~~不过想要换行貌似有点麻烦..."));
    else if(!enter && !ctrl)
        LEnterToSend->setText(tr("Alt+S发送消息，回车或Ctrl+回车换行..."));
    else if(enter && !ctrl)
        LEnterToSend->setText(tr("回车发送消息，Ctrl+回车换行..."));
    else
        LEnterToSend->setText(tr("Ctrl+回车发送消息，回车换行..."));

    trayIcon->setToolTip(windowTitle());

    logoChange();
}

void MainWidget::logoChange()
{
    QString fname=QFileInfo((cc->config)["LOGO_CACHE_PATH"].toString().arg(cc->myname)).absoluteFilePath();
    QSize size(60, 60);
    QMovie *Avatar = new QMovie(fname);
    Avatar->setScaledSize(size);
    ui->MyLogo->setMovie(Avatar);
    Avatar->start();
}

//public slots:
void MainWidget::onUList(QStringList &users)
{
    ui->UListWidget->clear();
    foreach(QString i,users)
    {
        ui->UListWidget->addItem(i);
    }
}

void MainWidget::onMsg(QString uname,QString msg)
{
    ui->MsgArea->append(tr("%1 : %2").arg(uname).arg(msg.toHtmlEscaped()));
    show();
    activateWindow();
    qApp->alert(this);
}

void MainWidget::on_ActAbout_triggered()
{
    QDesktopServices::openUrl(QUrl("http://0-ms.org/about"));
}

void MainWidget::on_ActMember_triggered()
{
    QFile file(":/Text/members.html");
    file.open(QFile::ReadOnly);
    QMessageBox msgBox;
    msgBox.resize(1000,500);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(tr("合作开发人员名单"));
    msgBox.setText(file.readAll());
    msgBox.exec();
}

void MainWidget::on_ActVer_triggered()
{
    QMessageBox::information(0,CLIENT_TITLE_NAME,tr("%1\n%2").arg(CLIENT_NAME).arg(CLIENT_VER_NAME));
}

void MainWidget::on_ActJyboxIndex_triggered()
{
    QDesktopServices::openUrl(QUrl("http://jybox.net"));
}

void MainWidget::on_ActCheckNew_triggered()
{
    QDesktopServices::openUrl(QUrl("http://0-ms.org/down"));
}

void MainWidget::on_ActDevBBS_triggered()
{
    QDesktopServices::openUrl(QUrl("http://jybox.net/bbs/thread.php?fid-19.html"));
}

//private:
void MainWidget::closeEvent(QCloseEvent *event)
{
    if(trayIcon && trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

bool MainWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->MsgEdit && event->type()==QEvent::KeyPress)
    {
        //如果是ui->MsgEdit发出的消息，且是按键消息
        int key=(static_cast<QKeyEvent*>(event))->key();
        if(key==Qt::Key_Return || key==Qt::Key_Enter)
        {
            //如果按键是回车
            if(!((static_cast<QKeyEvent*>(event))->modifiers() & Qt::ControlModifier))
            {
                //如果Ctrl没有被按下
                if((cc->config)["ENTER_TO_SEND"].toInt())
                {
                    on_DoSend_clicked();
                    return 1;
                }
            }
            else
            {
                //如果Ctrl被按下
                if((cc->config)["CTRL_ENTER_TO_SEND"].toInt())
                {
                    on_DoSend_clicked();
                    return 1;
                }
                else
                {
                    ui->MsgEdit->insertPlainText(tr("\n"));
                }
            }
        }
    }
    //将未处理的事件交给基类处理
    return QMainWindow::eventFilter(watched, event);
}

//private slots:
void MainWidget::onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            show();
            activateWindow();
            qApp->alert(this);
            break;
        default:
            break;
    }
}

void MainWidget::on_UListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    cc->showChatWidget(item->text());
}

void MainWidget::on_DoSend_clicked()
{
    if(ui->MsgEdit->toPlainText().isEmpty())
    {
        ui->MsgArea->append(tr(">>提示:请不要发送空消息"));
        return;
    }
    cc->msgCMsg(MAIN_GROUP,ui->MsgEdit->toPlainText());
    ui->MsgEdit->clear();
}

void MainWidget::on_ActChat_triggered()
{
    //这里是要打开所有选中的聊天窗体，下同
    QList<QListWidgetItem*> users=ui->UListWidget->selectedItems();
    for(QList<QListWidgetItem*>::Iterator i=users.begin();i!=users.end();++i)
    {
        cc->showChatWidget((*i)->text());
    }
}

void MainWidget::on_ActUserInfo_triggered()
{
    QList<QListWidgetItem*> users=ui->UListWidget->selectedItems();
    for(QList<QListWidgetItem*>::Iterator i=users.begin();i!=users.end();++i)
    {
        QDesktopServices::openUrl(QUrl((cc->config)["USERINFO_URL"].toString().arg((*i)->text())));
    }
}

void MainWidget::on_ActDebugOut_triggered()
{
    QDesktopServices::openUrl(QUrl((cc->config)["DEBUG_OUT_PATH"].toString()));
}

void MainWidget::on_ActQuit_triggered()
{
    delete trayIcon;//必须先销毁托盘图标，才能关闭主窗体
    qApp->closeAllWindows();//加这句是为了调用各个聊天窗体的析构函数
    qApp->quit();
}

void MainWidget::on_ActTray_triggered()
{
    trayIcon->showMessage(tr("已最小化到托盘"), CLIENT_TITLE_NAME, QSystemTrayIcon::Information, 5000);
    close();
}

void MainWidget::on_ActRefresh_triggered()
{
    cc->msgAskUList();
}

void MainWidget::on_ActOptionFile_triggered()
{
    QDesktopServices::openUrl(QUrl("./config.ini"));
}

void MainWidget::on_ActSource_triggered()
{
    QDesktopServices::openUrl(QUrl("http://0-ms.org/code"));
}

void MainWidget::on_ActProtocol_triggered()
{
    QDesktopServices::openUrl(QUrl("http://0-ms.org/wiki/protocol/p1-1"));
}

void MainWidget::on_ActChatLog_triggered()
{
    QDesktopServices::openUrl(QUrl((cc->config)["CHATLOG_PATH"].toString().arg(cc->myname).arg(QString(MAIN_GROUP).replace("*","_"))));
}
