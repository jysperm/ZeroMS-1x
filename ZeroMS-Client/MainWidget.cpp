#include "MainWidget.h"
#include "ui_MainWidget.h"
#include <QPushButton>
#include <QGroupBox>
#include <QMovie>
#include "FriendListItem.h"
#include "global.h"
#include "OClientCore.h"

MainWidget::MainWidget():ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    QSize size(ui->myAvatar->width(),ui->myAvatar->height());
    QMovie *avatar = new QMovie(":/images/0ms2logo.png");
    avatar->setScaledSize(size);
    ui->myAvatar->setMovie(avatar);
    avatar->start();

    ui->myName->setText("MyName");

    ui->friendList->init();

    on_actRefreshFriendList_triggered();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_actRefreshFriendList_triggered()
{
    core->main->AskUserList(core->uname,ALL,true);
}
