#include "MainWidget.h"
#include "ui_MainWidget.h"
#include <QPushButton>
#include <QGroupBox>
#include <QMovie>
#include "FriendListItem.h"

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
}

MainWidget::~MainWidget()
{
    delete ui;
}
