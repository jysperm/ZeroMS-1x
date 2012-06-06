#include <QMovie>
#include "FriendListItem.h"
#include "ui_FriendListItem.h"

FriendListItem::FriendListItem(QWidget *parent):QWidget(parent),ui(new Ui::FriendListItem)
{
    ui->setupUi(this);

    QSize size(ui->Avatar->width(),ui->Avatar->height());
    QMovie *avatar = new QMovie(":/images/0ms2logo.png");
    avatar->setScaledSize(size);
    ui->Avatar->setMovie(avatar);
    avatar->start();
}

FriendListItem::~FriendListItem()
{
    delete ui;
}
