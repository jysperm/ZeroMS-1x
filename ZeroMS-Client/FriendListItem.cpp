#include <QMovie>
#include "FriendListItem.h"
#include "ui_FriendListItem.h"

FriendListItem::FriendListItem(QString avatar,QString uname):ui(new Ui::FriendListItem)
{
    ui->setupUi(this);

    if(avatar.isEmpty())
        avatar=":/images/0ms2logo.png";

    QSize size(ui->avatar->width(),ui->avatar->height());
    QMovie *movie = new QMovie(avatar);
    movie->setScaledSize(size);
    ui->avatar->setMovie(movie);
    movie->start();

    setContentsMargins(0,0,0,0);

    setUName(uname);
}

FriendListItem::~FriendListItem()
{
    delete ui->avatar->movie();
    delete ui;
}
