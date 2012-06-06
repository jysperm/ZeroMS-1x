#include <QMovie>
#include "FriendListItem.h"
#include "ui_FriendListItem.h"

FriendListItem::FriendListItem(QString avatar,QString uname):ui(new Ui::FriendListItem)
{
    ui->setupUi(this);

    QSize size(ui->avatar->width(),ui->avatar->height());
    QMovie *movie = new QMovie(avatar);
    movie->setScaledSize(size);
    ui->avatar->setMovie(movie);
    movie->start();

    setUName(uname);
}

FriendListItem::~FriendListItem()
{
    delete ui->avatar->movie();
    delete ui;
}
